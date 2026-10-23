/*
 * A177492  Products of squares of 2 or more distinct primes.
 * https://oeis.org/A177492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 102, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A177492: Products of squares of 2 or more distinct primes.
   The term is the square of the n‑th squarefree integer (≥1) that has
   at least two distinct prime factors.  This implementation computes
   the value by generating all squarefree numbers up to a bound that
   is guaranteed to contain at least n such numbers, using a linear
   sieve for smallest prime factors. */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Rough upper bound: enough to cover n terms for n ≤ 10000. */
    idx_t limit = n * 20 + 3;

    while (1) {
        /* Smallest prime factor array. */
        uint32_t *spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
        if (!spf)
            return 0; /* allocation failure */

        /* Prime list for the linear sieve. */
        size_t primes_cap = limit / 10 + 10;
        uint32_t *primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        size_t primes_cnt = 0;

        for (idx_t i = 2; i <= limit; ++i) {
            if (spf[i] == 0) {
                spf[i] = (uint32_t)i;
                primes[primes_cnt++] = (uint32_t)i;
                if (primes_cnt == primes_cap) {
                    primes_cap <<= 1;
                    primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                    if (!primes) {
                        free(spf);
                        return 0;
                    }
                }
            }
            for (size_t j = 0; j < primes_cnt; ++j) {
                uint32_t p = primes[j];
                if (p > spf[i] || i * (idx_t)p > limit)
                    break;
                spf[i * (idx_t)p] = p;
            }
        }

        idx_t count = 0;
        for (idx_t m = 2; m <= limit; ++m) {
            idx_t x = m;
            idx_t distinct = 0;
            int squarefree = 1;
            while (x > 1) {
                uint32_t p = spf[x];
                x /= p;
                ++distinct;
                if (x % p == 0) { /* repeated prime factor */
                    squarefree = 0;
                    break;
                }
            }
            if (squarefree && distinct >= 2) {
                ++count;
                if (count == n) {
                    seq_t res = (seq_t)m * m;
                    free(spf);
                    free(primes);
                    return res;
                }
            }
        }

        free(spf);
        free(primes);
        /* Increase bound and retry. */
        limit <<= 1;
    }
}
