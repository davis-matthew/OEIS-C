/*
 * A078940  Row sums of A078938.
 * https://oeis.org/A078940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3 * Sum_{k=1..n} binomial(n-1,k-1) * a(n-k),
 *            with a(0) = 1. The recurrence follows directly from the OEIS
 *            entry for A078940
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of a(0)..a(max_cached)   */
    static idx_t max_cached = 0;    /* number of values stored so far */

    /* Ensure we can answer the request.  Grow the cache if necessary. */
    if (n >= max_cached) {
        idx_t new_size = n + 1;          /* need space for 0..n inclusive */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);

        /* Compute any missing terms, starting from the first missing index. */
        for (idx_t i = max_cached; i <= n; ++i) {
            if (i == 0) {                /* a(0) = 1 */
                cache[0] = (seq_t)1;
                continue;
            }

            seq_t sum   = 0;             /* partial sum Σ binom(i-1,t) * a(t)   */
            seq_t binom = 1;             /* binom(i-1,0) = 1                    */

            for (idx_t t = 0; t < i; ++t) {
                sum += cache[t] * binom;   /* accumulate term */

                /* Update binom to binom(i-1, t+1) for next iteration:
                   binom := binom * (i-1 - t) / (t+1)   */
                seq_t num = (seq_t)(i - 1 - t);   /* numerator fits in idx_t */
                seq_t den = (seq_t)(t + 1);        /* denominator fits in idx_t */
                binom = (binom * num) / den;
            }

            cache[i] = cache[i - 1] + (seq_t)3 * sum;   /* recurrence step */
        }

        max_cached = new_size;
    }

    return cache[n];
}
