/*
 * A082998  a(n) = card{ x <= n : omega(x) = 3 }.
 * https://oeis.org/A082998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = card{ x <= n : omega(x) = 3 } (number of integers ≤ n
 *            with exactly three distinct prime factors)
 * Range:     u8 n <= 944, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* The OEIS offset is 1; for n < 2 the answer is 0. */
    if (n < 2) return 0;

    /* A static array of smallest prime factors (spf) that grows
       as needed.  All callers see the same array, so it is reused.  */
    static uint64_t *spf = NULL;          /* spf[i] = smallest prime dividing i */
    static idx_t   spf_limit = 0;        /* last index for which spf was built */

    /* If n exceeds the current limit, rebuild spf up to n.  */
    if (n > spf_limit) {
        /* Allocate a new array of size n+1, zero‑initialized. */
        uint64_t *new_spf = NULL;

#ifdef __cplusplus
        new_spf = new uint64_t[n + 1](); /* value‑init to 0 */
#else
        new_spf = (uint64_t*)calloc(n + 1, sizeof(uint64_t));
#endif
        if (!new_spf) exit(EXIT_FAILURE);

        /* Sieve of Eratosthenes to fill spf. */
        for (idx_t i = 2; i <= n; ++i) {
            if (!new_spf[i]) {
                new_spf[i] = i;            /* i is prime */
                if (i > n / i) continue;   /* avoid overflow */
                for (idx_t j = i * i; j <= n; j += i) {
                    if (!new_spf[j]) new_spf[j] = i;
                }
            }
        }

        /* Replace the old array safely. */
#ifdef __cplusplus
        delete[] spf;
#else
        free(spf);
#endif
        spf = new_spf;
        spf_limit = n;
    }

    /* Count numbers ≤ n with exactly three distinct prime factors. */
    seq_t count = 0;

    for (idx_t i = 2; i <= n; ++i) {
        uint64_t x     = i;
        uint64_t last  = 0;
        idx_t    distinct = 0;

        /* Count distinct prime factors using spf. */
        while (x > 1) {
            uint64_t p = spf[x];
            if (p != last) {
                ++distinct;
                last = p;
            }
            /* Skip repeated powers of p. */
            do { x /= p; } while (x % p == 0);
        }

        if (distinct == 3) ++count;
    }

    return count;
}
