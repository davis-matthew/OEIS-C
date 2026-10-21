/*
 * A080687  Number of labeled n-element posets with no 3-element antichain.
 * https://oeis.org/A080687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence 2*a(n) + 2*(-5*n+6)*a(n-1)
 *            + 3*(n-1)*(3*n-4)*a(n-2) - 2*(n-1)*(n-2)*(2*n-3)*a(n-3) = 0 with
 *            initial values a(0)=1, a(1)=1, a(2)=3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n) {
    if (n < cache_len) return;
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = p;
        cache_cap = new_cap;
    }
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        switch (i) {
            case 0: cache[i] = (seq_t)1; break;
            case 1: cache[i] = (seq_t)1; break;
            case 2: cache[i] = (seq_t)3; break;
            default: {
                idx_t ni = (idx_t)i;
                seq_t sn = (seq_t)ni;
                seq_t a_nm1 = cache[i-1];
                seq_t a_nm2 = cache[i-2];
                seq_t a_nm3 = cache[i-3];

                /* c1 = (10*n-12)/2 , term1 = c1 * a(n-1)          */
                seq_t c1 = ((seq_t)10 * sn - (seq_t)12) / (seq_t)2;
                seq_t term1 = c1 * a_nm1;

                /* c3 = (n-1)*(n-2)*(2*n-3) , term3 = c3 * a(n-3)  */
                seq_t c3 = (sn - (seq_t)1) * (sn - (seq_t)2) *
                           ((seq_t)2 * sn - (seq_t)3);
                seq_t term3 = c3 * a_nm3;

                /* c2 = 3*(n-1)*(3*n-4)/2 , term2 = c2 * a(n-2)    */
                seq_t c2 = ((seq_t)3 * (sn - (seq_t)1) *
                           ((seq_t)3 * sn - (seq_t)4)) / (seq_t)2;
                seq_t term2 = c2 * a_nm2;

                cache[i] = term1 + term3 - term2;
            }
        }
    }
    cache_len = needed;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[(size_t)n];
}
