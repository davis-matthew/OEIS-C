/*
 * A202840  Number of secondary structures of size n having no stacks of
 *          length 1.
 * https://oeis.org/A202840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies * (n+4)*a(n) + (-2*n-5)*a(n-1) + (-n-1)*a(n-2) +
 *            2*(2*n-1)*a(n-3) * + (-n+2)*a(n-4) + 4*(-2*n+7)*a(n-5) +
 *            3*(n-5)*a(n-6) * + 3*(2*n-13)*a(n-7) + 2*(-n+8)*a(n-8) +
 *            2*(-2*n+19)*a(n-9) * + (n-11)*a(n-10) = 0, * with
 *            a(0)=1,a(1)=1,a(2)=1,a(3)=1,a(4)=1,a(5)=2,a(6)=4, *
 *            a(7)=8,a(8)=14,a(...
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 34, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initial values for n=0..10 */
    const seq_t init[11] = {1,1,1,1,1,2,4,8,14,23,38};

    /* ensure cache large enough */
    if (cache == NULL) {
        cache_size = 16;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < 11; ++i) cache[i] = init[i];
    } else if (n >= cache_size) {
        size_t new_size = cache_size;
        while ((idx_t)new_size <= n) new_size *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* return known initial values */
    if (n < 11) return cache[(size_t)n];

    /* compute new values up to n */
    for (idx_t k = 11; k <= n; ++k) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        /* coefficients for a(k-i) with i=1..10 */
        long long c1 = -2*(long long)k - 5;   /* negative */
        long long c2 = - (long long)k - 1;    /* negative */
        long long c3 = 4*(long long)k - 2;    /* positive for k>=11 */
        long long c4 = - (long long)k + 2;    /* negative */
        long long c5 = -8*(long long)k + 28;  /* negative for k>=11 */
        long long c6 = 3*(long long)k - 15;   /* positive for k>=6 */
        long long c7 = 6*(long long)k - 39;   /* positive for k>=7 */
        long long c8 = -2*(long long)k + 16;  /* negative for k>=9 */
        long long c9 = -4*(long long)k + 38;  /* negative for k>=10 */
        long long c10 = (long long)k - 11;    /* non‑negative for k>=11 */

        /* a(k-1) */
        seq_t term = (seq_t)(-c1) * cache[(size_t)(k-1)];
        sum_neg += term;

        /* a(k-2) */
        term = (seq_t)(-c2) * cache[(size_t)(k-2)];
        sum_neg += term;

        /* a(k-3) */
        term = (seq_t)c3 * cache[(size_t)(k-3)];
        sum_pos += term;

        /* a(k-4) */
        term = (seq_t)(-c4) * cache[(size_t)(k-4)];
        sum_neg += term;

        /* a(k-5) */
        term = (seq_t)(-c5) * cache[(size_t)(k-5)];
        sum_neg += term;

        /* a(k-6) */
        term = (seq_t)c6 * cache[(size_t)(k-6)];
        sum_pos += term;

        /* a(k-7) */
        term = (seq_t)c7 * cache[(size_t)(k-7)];
        sum_pos += term;

        /* a(k-8) */
        term = (seq_t)(-c8) * cache[(size_t)(k-8)];
        sum_neg += term;

        /* a(k-9) */
        term = (seq_t)(-c9) * cache[(size_t)(k-9)];
        sum_neg += term;

        /* a(k-10) */
        if (c10 >= 0) {
            term = (seq_t)c10 * cache[(size_t)(k-10)];
            sum_pos += term;
        } else {
            term = (seq_t)(-c10) * cache[(size_t)(k-10)];
            sum_neg += term;
        }

        /* a(k) = (sum_neg - sum_pos)/(k+4) */
        seq_t numerator = sum_neg - sum_pos;
        seq_t denom = (seq_t)(k + 4);
        cache[(size_t)k] = numerator / denom;
    }

    return cache[(size_t)n];
}
