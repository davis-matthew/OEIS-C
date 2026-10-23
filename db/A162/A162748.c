/*
 * A162748  Row sums of factorial-Pascal matrix A162747.
 * https://oeis.org/A162748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * a(n) = sum_{m=0..floor(n/2)} C(n, 2m) * 2^(n-2m) * (m)! * *
 *            Implementation: iteratively compute each term using the
 *            recurrence * that updates C(n,2m+2), 2^(n-2m-2) and m! in O(1)
 *            per term. * Results are cached so subsequent calls with larger n
 *            reuse what * has already been computed. The sequenc...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 32, u128 n <= 57,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static idx_t cache_size = 0;   /* size of allocated cache (number of stored entries) */
    static seq_t *cache = NULL;    /* array of already computed a(i) values */

    if (n >= cache_size) {
        /* need to extend cache up to n */
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure, return 0 (sequence values are >0 normally) */
        cache = tmp;
        cache_size = new_size;

        for (idx_t i = old_size; i < new_size; ++i) {
            /* hard‑coded initial values for safety */
            if (i == 0) { cache[i] = 1; continue; }
            if (i == 1) { cache[i] = 2; continue; }
            if (i == 2) { cache[i] = 5; continue; }
            if (i == 3) { cache[i] = 14; continue; }

            /* general computation for i ≥ 4 */
            seq_t sum     = 0;
            seq_t binom   = 1;                         /* C(i, 0) */
            seq_t fact    = 1;                         /* 0! = 1 */
            seq_t pow2_val = pow2(i);                  /* 2^i  → term for m=0 */
            seq_t term     = pow2_val;                 /* term for m=0 */
            sum = term;

            idx_t maxm = i / 2;                        /* maximum m such that 2m ≤ i */
            for (idx_t m = 0; m < maxm; ++m) {
                /* update binomial coefficient to C(i, 2m+2) */
                binom = binom * (seq_t)(i - 2 * m) * (seq_t)(i - 2 * m - 1);
                binom = binom / ((seq_t)(2 * m + 2) * (seq_t)(2 * m + 1));

                /* update factorial m! → (m+1)! */
                fact = fact * (seq_t)(m + 1);

                /* update power of 2: 2^(i-2m-2) from 2^(i-2m) */
                pow2_val = pow2_val >> 2;            /* divide by 4 */

                /* new term */
                term = binom * pow2_val * fact;
                sum += term;
            }
            cache[i] = sum;
        }
    }
    return cache[n];
}
