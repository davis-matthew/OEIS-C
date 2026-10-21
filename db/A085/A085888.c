/*
 * A085888  Let r and s be such that r + s = n; a(n) = minimum value of
 *          sigma(r) + sigma(s).
 * https://oeis.org/A085888
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 247, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A085888 */
/* a(n) = min_{r+s=n} (σ(r)+σ(s))  with offset 2  (n ≥ 2) */
#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n)
{
    /* sigma holds σ(k) for k = 0 … current_cap */
    static seq_t *sigma = NULL;
    static idx_t current_cap = 0;

    /* undefined for n < 2 – return 0 */
    if (n < 2)
        return 0;

    /* bring the σ table up to at least n */
    if (n > current_cap) {
        /* Allocate a new table and compute divisor sums */
        seq_t *new_sigma = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_sigma)
            return 0;        /* allocation failure – caller handles */

        /* initialise to zero */
        for (idx_t i = 0; i <= n; ++i)
            new_sigma[i] = 0;

        /* sieve–like accumulation of divisor sums */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i)
                new_sigma[j] += (seq_t)i;
        }

        /* replace old table */
        free(sigma);
        sigma = new_sigma;
        current_cap = n;
    }

    /* initialise best with the first partition (1, n‑1) */
    seq_t best = sigma[1] + sigma[n - 1];

    /* try all r from 2 to n/2 (the rest are symmetric) */
    for (idx_t r = 2; r <= n / 2; ++r) {
        seq_t val = sigma[r] + sigma[n - r];
        if (val < best)
            best = val;
    }

    return best;
}
