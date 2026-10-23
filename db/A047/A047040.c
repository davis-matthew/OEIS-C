/*
 * A047040  Array T read by diagonals: T(h,k)=number of paths consisting of
 *          steps from (0,0) to (h,k) such that each step has length 1
 *          directed up or right and no step touches the line y=3x/4 unless
 *          x=0 or x=h.
 * https://oeis.org/A047040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   alloc2d, idx_to_coord
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of lattice paths from (0,0) to (h,k)
 * where n = (h+k)(h+k+1)/2 + h and each step is either up (0,1)
 * or right (1,0) and no unit segment intersects the line y = 3x/4
 * except at the start or at the final point.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc2d.h"
#include "../utils/idx_to_coord.h"

static seq_t **dp_above = NULL;
static seq_t **dp_below = NULL;
static bool initialized = false;
static idx_t max_d = 0;

/* Initialization of DP tables */
static void init_dp(void)
{
    if (initialized) return;
    initialized = true;

    /* maximal diagonal needed for n <= 10000 */
    max_d = 0;
    while ((max_d + 1) * (max_d + 2) / 2 <= 10000) ++max_d;

    dp_above = alloc2d(max_d + 1, max_d + 1);
    dp_below = alloc2d(max_d + 1, max_d + 1);
    if (!dp_above || !dp_below) return;              /* allocation failure */

    for (idx_t h = 0; h <= max_d; ++h) {
        for (idx_t k = 0; k <= max_d - h; ++k) {
            int64_t D = (int64_t)4 * k - (int64_t)3 * h;
            if (h == 0 && k == 0) {
                dp_above[0][0] = 1;
                dp_below[0][0] = 1;
            } else if (D > 0) {
                seq_t val = 0;
                if (h > 0) val += dp_above[h - 1][k];
                if (k > 0) val += dp_above[h][k - 1];
                dp_above[h][k] = val;
            } else if (D < 0) {
                seq_t val = 0;
                if (h > 0) val += dp_below[h - 1][k];
                if (k > 0) val += dp_below[h][k - 1];
                dp_below[h][k] = val;
            } /* D==0 points are not stored */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    init_dp();

    idx_t h, k;
    idx_to_coord(n, &h, &k);
    int64_t D = (int64_t)4 * k - (int64_t)3 * h;

    if (D > 0) {
        return dp_above[h][k];
    } else if (D < 0) {
        return dp_below[h][k];
    } else { /* D == 0, h>0 or k>0 */
        seq_t res = 0;
        if (h > 0) res += dp_above[h - 1][k];
        if (k > 0) res += dp_below[h][k - 1];
        return res;
    }
}
