/*
 * A195595  Number of ways to place 7n nonattacking kings on a vertical
 *          cylinder 14 X 2n.
 * https://oeis.org/A195595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -4*a(n-8) + 28*a(n-7) - 85*a(n-6) + 146*a(n-5)
 *            -155*a(n-4) + 104*a(n-3) - 43*a(n-2) + 10*a(n-1), with initial
 *            values: a(1)=256, a(2)=6060, a(3)=58776, a(4)=358564,
 *            a(5)=1649420, a(6)=6286658, a(7)=20984924, a(8)=63558566
 * Range:     u8 none, u16 n <= 3, u32 n <= 12, u64 n <= 41, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

static const seq_t init[9] = {
    0UL,          /* unused index 0 */
    256UL,        /* a(1) */
    6060UL,       /* a(2) */
    58776UL,      /* a(3) */
    358564UL,     /* a(4) */
    1649420UL,    /* a(5) */
    6286658UL,    /* a(6) */
    20984924UL,   /* a(7) */
    63558566UL    /* a(8) */
};

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined below offset */

    size_t needed = (size_t)n + 1;        /* array index up to n inclusive */

    if (!cache || needed > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);   /* allocation failed */
        cache = new_cache;

        /* Compute any newly needed entries */
        for (size_t i = cache_len; i < needed; ++i) {
            if (i == 0) continue;          /* skip unused 0 index */
            if (i <= 8) {
                cache[i] = init[i];
            } else {
                /* Recurrence split to avoid negative intermediates:
                   a = (10*u8 + 104*u6 + 146*u4 + 28*u2)
                       - (4*u1 + 43*u7 + 85*u5 + 155*u3)  */
                seq_t sum1 = 10 * cache[i-1]
                           + 104 * cache[i-3]
                           + 146 * cache[i-5]
                           + 28  * cache[i-7];
                seq_t sum2 = 4   * cache[i-8]
                           + 43  * cache[i-2]
                           + 85  * cache[i-6]
                           + 155 * cache[i-4];
                cache[i] = sum1 - sum2;
            }
        }
        cache_len = needed;
    }

    return cache[n];
}
