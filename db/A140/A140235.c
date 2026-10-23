/*
 * A140235  Partial sum of non-semiprimes A100959.
 * https://oeis.org/A140235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum{k=1..n} A100959(k) (the sum of the first n
 *            non‑semiprimes). Implementation uses a simple SPF (smallest
 *            prime factor) sieve to test whether a number has exactly two
 *            prime factors with multiplicity; if not, it contributes to the
 *            accumulated sum
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Start with a reasonably sized upper bound for the sieve.   */
    idx_t limit = n * 2 + 100;  /* will be doubled if insufficient */

    for (;;) {
        /* Allocate an array for the smallest prime factor of each
           integer up to `limit`.  Cast explicitly for C++ compatibility. */
        uint32_t *spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        if (!spf)
            return (seq_t)0;   /* allocation failure */

        /* initialise spf[i] = i */
        for (idx_t i = 0; i <= limit; ++i)
            spf[i] = (uint32_t)i;

        /* sieve to fill spf */
        idx_t sq = 0;
        while ((sq + 1) * (sq + 1) <= limit)
            ++sq;
        for (idx_t i = 2; i <= sq; ++i) {
            if (spf[i] == (uint32_t)i) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == (uint32_t)j)
                        spf[j] = (uint32_t)i;
                }
            }
        }

        /* Scan numbers in increasing order, accumulating the sum of
           non‑semiprimes until we reach the desired count. */
        seq_t sum = (seq_t)0;
        idx_t count = 0;

        for (idx_t i = 1; i <= limit; ++i) {
            bool semiprime = false;

            if (i >= 2) {
                idx_t x = i;
                idx_t fac_cnt = 0;
                while (x > 1 && fac_cnt <= 2) {
                    x /= spf[x];
                    ++fac_cnt;
                }
                if (fac_cnt == 2)
                    semiprime = true;
            }

            if (!semiprime) {
                ++count;
                sum += (seq_t)i;    /* cast `i` to the seq_t type */
                if (count == n) {
                    free(spf);
                    return sum;
                }
            }
        }

        /* Not enough non‑semiprimes were found – double the limit and
           try again. */
        free(spf);
        limit *= 2;
    }
}
