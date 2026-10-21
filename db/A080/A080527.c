/*
 * A080527  Expansion of e.g.f. exp(3*cosh(x))/e^3 (even powers only).
 * https://oeis.org/A080527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(n) = 3 * Sum_{k=1..n} binomial(2*n-1, 2*k-1) *
 *            a(n-k) for n ≥ 1 (only even powers of the e.g.f. are taken)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for already computed terms */
    static seq_t *cache = NULL;   /* array of a[0 .. last_computed]   */
    static idx_t last_computed = 0;   /* index of the largest computed term */

    /* Allocate the cache if this is the first call   */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!cache) exit(1);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        last_computed = 0;
    }

    /* Extend the cache only when we need a higher index   */
    if (n > last_computed) {
        /* Ensure the cache is large enough   */
        idx_t new_size = (n + 1 > 2 ? n + 1 : 2);
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!cache) exit(1);

        /* Compute a[last_computed+1] .. a[n]   */
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t acc = 0;                      /* accumulator for the sum   */
            seq_t cur_binom = (seq_t)(2 * i - 1);/* binom(2i-1,1) for k=1      */

            /* k = 1 term: binom * a[i-1]          */
            acc += cur_binom * cache[i - 1];

            /* Subsequent odd binom coefficients   */
            for (idx_t k = 2; k <= i; ++k) {
                idx_t prev_k = k - 1;

                /* Ratio to get the next odd binomial coefficient */
                seq_t num1 = (seq_t)2 * (i - prev_k);
                seq_t num2 = num1 - 1;
                seq_t den1 = (seq_t)2 * prev_k;
                seq_t den2 = den1 + 1;

                /* cur_binom currently equals binom(2i-1,2k-3)   */
                cur_binom = cur_binom * num1 / den1;
                cur_binom = cur_binom * num2 / den2;        /* now binom(2i-1,2k-1) */

                acc += cur_binom * cache[i - k];
            }

            cache[i] = (seq_t)3 * acc;
        }

        last_computed = n;
    }

    return cache[n];
}
