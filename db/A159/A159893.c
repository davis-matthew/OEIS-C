/*
 * A159893  Positive numbers y such that y^2 is of the form x^2+(x+727)^2 with
 *          integer x.
 * https://oeis.org/A159893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3)-a(n-6) for n>6; a(1)=677, a(2)=727, a(3)=785,
 *            a(4)=3277, a(5)=3635, a(6)=4033
 * Range:     u8 none, u16 n <= 9, u32 n <= 27, u64 n <= 66, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_vals[6] = {677, 727, 785, 3277, 3635, 4033};

/* Cache up to a reasonably large n; enough for requested limits. */
static seq_t cache[20005];
static size_t cache_len = 0;   /* highest index computed so far (0 means none) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, return 0 */

    /* Compute any missing terms up to n. */
    if ((size_t)n > cache_len) {
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            if (i <= 6) {
                cache[i] = init_vals[i - 1];
            } else {
                cache[i] = 6 * cache[i - 3] - cache[i - 6];
            }
        }
        cache_len = (size_t)n;
    }
    return cache[n];
}
