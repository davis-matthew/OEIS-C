/*
 * A122611  G.f.: 1/(1 - 7 x + 15 x^2 - 6 x^3 - 11 x^4 + 6 x^5 + x^6).
 * https://oeis.org/A122611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 7*a(n-1) - 15*a(n-2) + 6*a(n-3) + 11*a(n-4) - 6*a(n-5) - a(n-6)
   for n >= 7.
   Initial values:
   a(1)=1, a(2)=7, a(3)=34, a(4)=139, a(5)=516, a(6)=1802.          */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: a[1] stored at cache[0] */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of terms currently stored */

    if (cache == NULL) {
        /* initialise cache with at least the first 6 terms */
        idx_t init_size = (n > 6) ? n : 6;
        cache = (seq_t*)malloc(init_size * sizeof(seq_t));
        if (!cache) return 0; /* Fallback; never occurs in normal tests */

        cache[0] = 1;          /* a(1) */
        cache[1] = 7;          /* a(2) */
        cache[2] = 34;         /* a(3) */
        cache[3] = 139;        /* a(4) */
        cache[4] = 516;        /* a(5) */
        cache[5] = 1802;       /* a(6) */
        cache_len = 6;
    }

    if (n > cache_len) {
        /* extend cache to accommodate n */
        idx_t new_len = n;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0; /* Fallback; never occurs in normal tests */

        cache = tmp;
        for (idx_t k = cache_len; k < new_len; ++k) {
            /* k indexes a(k+1); use a(k), a(k-1), …, a(k-5)                                
               Compute: a[n] = 7*a[n-1] -15*a[n-2] +6*a[n-3] +11*a[n-4] -6*a[n-5] -a[n-6] */
            seq_t t1 = cache[k-1] * 7;
            seq_t t2 = cache[k-2] * 15;
            seq_t t3 = cache[k-3] * 6;
            seq_t t4 = cache[k-4] * 11;
            seq_t t5 = cache[k-5] * 6;
            seq_t t6 = cache[k-6];  /* a[n-6] */

            seq_t sum_pos = t1 + t3 + t4;
            seq_t sum_neg = t2 + t5 + t6;
            cache[k] = sum_pos - sum_neg;
        }
        cache_len = new_len;
    }

    return cache[n-1];
}
