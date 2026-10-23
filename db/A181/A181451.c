/*
 * A181451  Numbers k such that 13 is the largest prime factor of k^2 - 1.
 * https://oeis.org/A181451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A181451 – numbers k such that 13 is the largest prime factor of k^2−1 */

#include <stdlib.h>
#include <stdbool.h>

/* Definition: a(n) is the nth integer k (starting from 1) for which the
   largest prime factor of k^2−1 equals 13. The sequence is finite and all
   terms are ≤ 246401. This implementation pre‑computes the list on the
   first call and then answers arbitrary indexed queries. */

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *terms   = NULL;
    static idx_t  term_cnt = 0;

    if (!initialized) {
        /* Upper bound for k from A175607(6) */
        const idx_t max_k = 246401;
        /* primes up to 13 to test divisibility */
        const uint64_t primes[6] = {2, 3, 5, 7, 11, 13};

        /* Initial capacity guess */
        size_t cap = 64;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms) return 0; /* allocation failed */

        size_t count = 0;

        for (idx_t k = 2; k <= max_k; ++k) {
            /* k must be ≡ ±1 (mod 13) so that 13 divides k^2−1 */
            if ((k % 13) != 1 && (k % 13) != 12)
                continue;

            bool ok = true;
            for (int side = 0; side < 2 && ok; ++side) {
                uint64_t val = (side == 0) ? (uint64_t)(k - 1)
                                           : (uint64_t)(k + 1);
                for (int i = 0; i < 6; ++i) {
                    uint64_t p = primes[i];
                    while (val % p == 0)
                        val /= p;
                }
                if (val > 1)      /* remaining factor > 13 */
                    ok = false;
            }

            if (ok) {
                if (count == cap) {
                    cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                    if (!tmp) { free(terms); return 0; }
                    terms = tmp;
                }
                terms[count++] = (seq_t)k;
            }
        }

        term_cnt = (idx_t)count;
        initialized = true;
    }

    if (n == 0 || n > term_cnt) return 0;
    return terms[n - 1];
}
