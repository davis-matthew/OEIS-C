/*
 * A041214  Numerators of continued fraction convergents to sqrt(118).
 * https://oeis.org/A041214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerators of the continued‑fraction convergents to
 *            sqrt(118). The partial quotients are: a0 = 10 a1.. =
 *            [1,6,3,2,10,2,3,6,1,20] repeating. Recurrence: p_k = a_k *
 *            p_{k-1} + p_{k-2} with p_{-1} = 1, p_{-2} = 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a static cache so that any call order works,
   and it keeps the implementation O(n) in the worst case. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of elements stored in cache   */
    static size_t cache_cap = 0; /* allocated capacity of cache array    */

    /* Allocate or grow cache if necessary. */
    if (n >= cache_len) {
        size_t need = (size_t)n + 1;
        if (need > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
            if (!tmp) return (seq_t)0;         /* allocation failed */
            cache = tmp;
            cache_cap = need;
        }

        /* Determine the last two convergents needed for the recurrence. */
        seq_t p_im2, p_im1;                /* p_{k-1} and p_{k-2} for next calculation */

        if (cache_len == 0) {
            p_im1 = (seq_t)1;  /* p_{-1} */
            p_im2 = (seq_t)0;  /* p_{-2} */
        } else if (cache_len == 1) {
            p_im1 = cache[0];  /* p0 */
            p_im2 = (seq_t)1;  /* p_{-1} */
        } else {
            p_im1 = cache[cache_len - 1];
            p_im2 = cache[cache_len - 2];
        }

        /* Period of the continued fraction after a0. */
        static const seq_t period[10] = {1, 6, 3, 2, 10, 2, 3, 6, 1, 20};
        const size_t period_len = 10;

        /* Compute missing convergents up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t ai;
            if (i == 0) {
                ai = (seq_t)10;
            } else {
                ai = period[(i - 1) % period_len];
            }
            seq_t next = ai * p_im1 + p_im2;
            cache[i] = next;

            /* Shift for next iteration. */
            p_im2 = p_im1;
            p_im1 = next;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
