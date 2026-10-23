/*
 * A143478  Expansion of 1/(x^10*p(1/x)), where p(x) = x^11 + x^10 - 11*x^9 -
 *          11*x^8 + 42*x^7 + 40*x^6 - 66*x^5 - 54*x^4 + 42*x^3 + 24*x^2 - 8*x
 *          - 1 is a Salem polynomial.
 * https://oeis.org/A143478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1) + 11*a(n-2) + 11*a(n-3) - 42*a(n-4) - 40*a(n-5) +
 *            66*a(n-6) + 54*a(n-7) - 42*a(n-8) - 24*a(n-9) + 8*a(n-10) +
 *            a(n-11) (n>=11) with a(0)=0, a(1)=1, a(2)=-1, a(3)=12, a(4)=-12,
 *            a(5)=91, a(6)=-89, a(7)=560, a(8)=-526, a(9)=3061, a(10)=-2715
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 26, i64 n <= 57, i128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initial values for n = 0..10 */
    if (cache_size == 0) {
        cache_size = 11;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-1;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)-12;
        cache[5] = (seq_t)91;
        cache[6] = (seq_t)-89;
        cache[7] = (seq_t)560;
        cache[8] = (seq_t)-526;
        cache[9] = (seq_t)3061;
        cache[10] = (seq_t)-2715;
    }

    /* extend cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* realloc may return NULL if allocation fails – ignore error handling per requirements */
        cache = new_cache;
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] =
                -cache[i-1]                /* -a(n-1) */
                + 11 * cache[i-2]          /* + 11*a(n-2) */
                + 11 * cache[i-3]          /* + 11*a(n-3) */
                - 42 * cache[i-4]          /* - 42*a(n-4) */
                - 40 * cache[i-5]          /* - 40*a(n-5) */
                + 66 * cache[i-6]          /* + 66*a(n-6) */
                + 54 * cache[i-7]          /* + 54*a(n-7) */
                - 42 * cache[i-8]          /* - 42*a(n-8) */
                - 24 * cache[i-9]          /* - 24*a(n-9) */
                + 8  * cache[i-10]         /* + 8*a(n-10) */
                +     cache[i-11];         /* + a(n-11)  */
        }
        cache_size = new_size;
    }

    return cache[n];
}
