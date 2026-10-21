/*
 * A054785  a(n) = sigma(2n) - sigma(n), where sigma is the sum of divisors of
 *          n, A000203.
 * https://oeis.org/A054785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(2n) - sigma(n)
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for accumulated sigma values */
static seq_t *sigma = NULL;
static idx_t max_limit = 0;

/* calculate a(n) = sigma(2n) - sigma(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n=0, return 0 */

    idx_t needed = n * 2;              /* we need sigma up to 2*n */

    /* create or resize the sigma array if necessary */
    if (needed > max_limit) {
        idx_t new_limit = needed;
        seq_t *new_sigma = (seq_t *)calloc(new_limit + 1, sizeof(seq_t));
        if (!new_sigma)
            return 0;                  /* allocation failure → 0 */

        /* compute divisor sums via a simple sieve */
        for (idx_t i = 1; i <= new_limit; ++i) {
            seq_t add = (seq_t)i;
            for (idx_t j = i; j <= new_limit; j += i)
                new_sigma[j] += add;
        }

        free(sigma);                  /* discard old data */
        sigma = new_sigma;
        max_limit = new_limit;
    }

    return sigma[2 * n] - sigma[n];
}
