/*
 * A210695  a(n) = 6*a(n-1) - a(n-2) + 6 with n>1, a(0)=0, a(1)=1.
 * https://oeis.org/A210695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n)=6*a(n-1)-a(n-2)+6 for n>1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t  cap   = 0;
static idx_t  last  = (idx_t)-1;   /* highest index already computed */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cap   = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        last    = 1;
    }

    /* grow the array if necessary */
    if (n >= cap) {
        size_t newcap = (size_t)n + 1;
        if (newcap < 2) newcap = 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* compute any missing values up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            /* 6*a(i-1) computed by repeated addition to avoid overflow */
            seq_t temp = cache[i-1];
            for (int k = 1; k < 6; ++k) {
                temp += cache[i-1];
            }
            temp -= cache[i-2];
            temp += (seq_t)6;
            cache[i] = temp;
        }
        last = n;
    }

    return cache[n];
}
