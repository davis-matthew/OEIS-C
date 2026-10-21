/*
 * A054820  Second term of weak prime quartet: p(m)-p(m-1) < p(m+1)-p(m) <
 *          p(m+2)-p(m+1).
 * https://oeis.org/A054820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A054820: second term of weak prime quartet.
   The function generates primes sequentially, keeps a sliding window
   of the last four, and stores the second prime of each quartet
   that satisfies p(i)-p(i-1) < p(i+1)-p(i) < p(i+2)-p(i+1). */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static state across calls */
    static bool initialized = false;
    static idx_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static idx_t *weak_primes = NULL;
    static size_t weak_cnt = 0, weak_cap = 0;
    static idx_t next_candidate = 11;    /* first odd candidate after 7 */

    if (!initialized) {
        /* Prime storage */
        primes_cap = 16;
        primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
        primes[0] = 2;
        primes[1] = 3;
        primes[2] = 5;
        primes[3] = 7;
        primes_cnt = 4;

        /* Weak-prime storage */
        weak_cap = 16;
        weak_primes = (idx_t *)malloc(weak_cap * sizeof(idx_t));
        weak_cnt = 0;

        initialized = true;
    }

    /* Generate primes until we have at least n weak primes */
    while (weak_cnt < (size_t)n) {
        /* Find next prime */
        idx_t cand = next_candidate;
        for (;;) {
            bool is_prime = true;
            for (size_t i = 0; i < primes_cnt; ++i) {
                idx_t p = primes[i];
                if (p > cand / p) break;          /* p*p > cand */
                if (cand % p == 0) { is_prime = false; break; }
            }
            if (is_prime) break;
            cand += 2;                              /* try next odd */
        }

        /* Store the new prime */
        if (primes_cnt == primes_cap) {
            primes_cap = primes_cap * 2;
            primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
        }
        primes[primes_cnt++] = cand;
        next_candidate = cand + 2;   /* next odd candidate */

        /* Check for weak quartet if we have at least four primes */
        if (primes_cnt >= 4) {
            idx_t g1 = primes[primes_cnt - 3] - primes[primes_cnt - 4];
            idx_t g2 = primes[primes_cnt - 2] - primes[primes_cnt - 3];
            idx_t g3 = primes[primes_cnt - 1] - primes[primes_cnt - 2];
            if (g1 < g2 && g2 < g3) {
                /* Append the second prime of the quartet */
                if (weak_cnt == weak_cap) {
                    weak_cap = weak_cap * 2;
                    weak_primes = (idx_t *)realloc(weak_primes,
                                                   weak_cap * sizeof(idx_t));
                }
                weak_primes[weak_cnt++] = primes[primes_cnt - 3];
            }
        }
    }

    /* Return the requested term (1‑based index) */
    return (seq_t)weak_primes[n - 1];
}
