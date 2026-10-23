/*
 * A015023  q-factorial numbers for q=-9.
 * https://oeis.org/A015023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Product_{k=1..n} ((-9)^k - 1) / (-10) Initial value: a(0)
 *            = 1 Recurrence: a(n) = (((-9)^n - 1) * a(n-1)) / (-10) for n >=
 *            1
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached sequence values */
static idx_t cache_len = 0;   /* number of terms currently cached   */
static seq_t pow_last = 1;    /* (-9)^(cache_len-1);  pow_last=1 when cache_len=1 (a(0)) */

seq_t a(idx_t n)
{
    /* ensure the zero-th term is cached */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        pow_last = (seq_t)1;   /* (-9)^0 = 1  */
    }

    if (n < cache_len)
        return cache[n];

    /* compute terms from cache_len up to n (inclusive) */
    seq_t pow = pow_last * ((seq_t)(-9));            /* (-9)^(cache_len)     */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t factor = (pow - (seq_t)1) / ((seq_t)(-10));
        seq_t new_term = cache[k - 1] * factor;

        /* grow the cache to hold the new term */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (k + 1));
        cache = tmp;
        cache[k] = new_term;

        pow_last = pow;          /* store (-9)^k for next expansion          */
        pow     = pow * ((seq_t)(-9));  /* pow becomes (-9)^(k+1) for next loop */
    }

    cache_len = n + 1;
    return cache[n];
}
