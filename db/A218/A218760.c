/*
 * A218760  Unmatched value maps: number of nX3 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal or
 *          vertical neighbor in a random 0..1 nX3 array.
 * https://oeis.org/A218760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence of order 19 * * a(n) = 4*a(n-1) +
 *            a(n-2) + 7*a(n-3) +11*a(n-4) + 2*a(n-5) * -28*a(n-6)-11*a(n-7)-
 *            4*a(n-8)-13*a(n-9)+12*a(n-10) *
 *            -137*a(n-11)-48*a(n-12)+129*a(n-13)+ 31*a(n-14) * -
 *            51*a(n-15)-32*a(n-16)+ 6*a(n-17)+10*a(n-18)+ 2*a(n-19) * with
 *            initial values a(1) … a(1...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const int COEFF[19] = {
    4, 1, 7, 11, 2, -28, -11, -4, -13, 12, -137, -48, 129, 31, -51, -32, 6, 10, 2
};

static const seq_t INIT[20] = {
    0, /* dummy for index 0 */
    (seq_t)4ULL,
    (seq_t)18ULL,
    (seq_t)90ULL,
    (seq_t)403ULL,
    (seq_t)1876ULL,
    (seq_t)8697ULL,
    (seq_t)40383ULL,
    (seq_t)187413ULL,
    (seq_t)869598ULL,
    (seq_t)4035492ULL,
    (seq_t)18727387ULL,
    (seq_t)86907272ULL,
    (seq_t)403304737ULL,
    (seq_t)1871588753ULL,
    (seq_t)8685363409ULL,
    (seq_t)40305616278ULL,
    (seq_t)187043701224ULL,
    (seq_t)868001747571ULL,
    (seq_t)4028080274884ULL
};

/* Cache of computed values */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static idx_t  last_computed = 0;

seq_t a(idx_t n)
{
    /* initialise cache if needed */
    if (!cache) {
        cache_len    = 20;                           /* 0..19 */
        cache        = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = 0;
        for (size_t i = 1; i <= 19; ++i)
            cache[i] = INIT[i];
        last_computed = 19;
    }

    if (n <= last_computed)
        return cache[n];

    /* grow cache if necessary */
    if (n + 1 > cache_len) {
        size_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }

    /* compute terms from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 0; k < 19; ++k) {
            idx_t idx = i - 1 - k;                 /* i-1-k ≥ 1 here */
            seq_t prod = cache[idx] * (seq_t)COEFF[k];
            sum += prod;
        }
        cache[i] = sum;
    }

    last_computed = n;
    return cache[n];
}
