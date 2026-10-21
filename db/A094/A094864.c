/*
 * A094864  a(0)=1, a(1)=2, a(2)=6, a(3)=18; for n >= 4, a(n) = 6*a(n-1) -
 *          11*a(n-2) + 6*a(n-3) - a(n-4).
 * https://oeis.org/A094864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=6, a(3)=18,
 *            a(n)=6*a(n-1)-11*a(n-2)+6*a(n-3)-a(n-4) for n≥4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static idx_t a_cache_len = 0;

seq_t a(idx_t n)
{
    /* handle trivial initial values directly */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)2;
    if (n == 2U) return (seq_t)6;
    if (n == 3U) return (seq_t)18;

    /* ensure cache is large enough to hold a(n) */
    if (n >= a_cache_len) {
        size_t new_len   = (size_t)(n + 1);
        size_t old_len   = (size_t)a_cache_len;
        seq_t *tmp   = NULL;

        if (a_cache == NULL) {
            /* first allocation: reserve space for n+1 values */
            tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (!tmp) exit(1);

            /* initialise the four fixed entries */
            tmp[0] = (seq_t)1;
            tmp[1] = (seq_t)2;
            tmp[2] = (seq_t)6;
            tmp[3] = (seq_t)18;

            /* compute all terms up to n */
            for (size_t i = 4; i <= new_len - 1; ++i) {
                seq_t t1 = 6 * tmp[i-1];
                seq_t t2 = 11 * tmp[i-2];
                seq_t t3 = 6 * tmp[i-3];
                tmp[i] = t1 + t3 - t2 - tmp[i-4];
            }
            a_cache      = tmp;
            a_cache_len  = (idx_t)new_len;
        } else {
            /* grow existing cache */
            tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
            if (!tmp) exit(1);

            for (size_t i = old_len; i < new_len; ++i) {
                seq_t t1 = 6 * tmp[i-1];
                seq_t t2 = 11 * tmp[i-2];
                seq_t t3 = 6 * tmp[i-3];
                tmp[i] = t1 + t3 - t2 - tmp[i-4];
            }
            a_cache      = tmp;
            a_cache_len  = (idx_t)new_len;
        }
    }

    return a_cache[n];
}
