/*
 * A128730  Number of UDL's in all skew Dyck paths of semilength n.
 * https://oeis.org/A128730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the D‑finite recurrence +2*(n-1)*(3*n-8)*a(n)
 *            +(-39*n^2+161*n-148)*a(n-1) +(48*n^2-215*n+220)*a(n-2)
 *            -5*(3*n-5)*(n-3)*a(n-3)=0, with a(0)=0, a(1)=0, a(2)=1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* memoised cache for all n seen so far */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n >= cache_len) {
        /* grow cache to hold indices 0..n */
        size_t old_len = cache_len;
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            cache = NULL;
            cache_len = 0;
            return (seq_t)0;
        }
        cache = tmp;
        cache_len = new_len;

        /* initialise first three entries if needed */
        if (new_len > 0) cache[0] = (seq_t)0;
        if (new_len > 1) cache[1] = (seq_t)0;
        if (new_len > 2) cache[2] = (seq_t)1;

        /* compute new values */
        idx_t start = (old_len > 3 ? old_len : 3);
        for (idx_t i = start; i <= n; ++i) {
            /* coefficients as 64‑bit integers */
            long long coeff1 = -39LL * i * i + 161LL * i - 148LL;
            long long coeff2 = 48LL * i * i - 215LL * i + 220LL;
            long long coeff3 = -5LL * (3LL * i - 5LL) * (i - 3LL);
            long long denom_val = 2LL * (i - 1LL) * (3LL * i - 8LL); /* positive for i>=3 */

            if (sizeof(seq_t) <= 8) {
                /* use 128‑bit intermediate to avoid overflow on fixed types */
                signed __int128 t1 = (signed __int128)coeff1 * (signed __int128)cache[i - 1];
                signed __int128 t2 = (signed __int128)coeff2 * (signed __int128)cache[i - 2];
                signed __int128 t3 = (signed __int128)coeff3 * (signed __int128)cache[i - 3];
                signed __int128 numer = -(t1 + t2 + t3);
                seq_t denom = (seq_t)denom_val;
                cache[i] = (seq_t)(numer / denom);
            } else {
                /* seq_t is an arbitrary‑precision type – use its operators */
                seq_t t1 = (seq_t)coeff1 * cache[i - 1];
                seq_t t2 = (seq_t)coeff2 * cache[i - 2];
                seq_t t3 = (seq_t)coeff3 * cache[i - 3];
                seq_t numer = -(t1 + t2 + t3);
                seq_t denom = (seq_t)denom_val;
                cache[i] = numer / denom;
            }
        }
    }
    return cache[n];
}
