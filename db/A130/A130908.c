/*
 * A130908  E.g.f.: exp(x+x^2/2!+x^3/3!)/(1-x).
 * https://oeis.org/A130908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) satisfies the recurrence
 * +2*a(n) +2*(-n-1)*a(n-1) +(n-1)*(n-2)*a(n-3) +(n-1)*(n-2)*(n-3)*a(n-4)=0
 * with initial values a(0)=1, a(1)=2, a(2)=6, a(3)=23.
 *
 * The implementation caches computed values so that calls may be in any order.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t  cache_max = (idx_t)-1;   /* index of last computed term */

/* ensure the cache can hold at least n+1 terms */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        size_t need = (n < 4) ? 4 : (size_t)n + 1;
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (cache == NULL) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)23;
        cache_max = 3;
    }
    size_t need = (size_t)n + 1;
    if (need > cache_max + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 3) {
        ensure_cache(n);
        return cache[n];
    }

    ensure_cache(n);

    /* compute any missing terms up to n */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* terms needed from the recurrence */
            seq_t a_nm1 = cache[i - 1];
            seq_t a_nm3 = cache[i - 3];
            seq_t a_nm4 = cache[i - 4];

            /* (n+1)*a(n-1)  */
            seq_t term = ((seq_t)(i + 1)) * a_nm1;

            /* ((n-1)*(n-2)*a(n-3))/2  */
            seq_t t3_num = ((seq_t)(i - 1)) * (i - 2) * a_nm3;
            seq_t t3 = t3_num / 2;

            /* ((n-1)*(n-2)*(n-3)*a(n-4))/2  */
            seq_t t4_num = ((seq_t)(i - 1)) * (i - 2) * (i - 3) * a_nm4;
            seq_t t4 = t4_num / 2;

            cache[i] = term - t3 - t4;
        }
        cache_max = n;
    }

    return cache[n];
}
