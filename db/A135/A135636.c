/*
 * A135636  Values of x in positive solutions (x,y,z) to the Diophantine
 *          43x+7y+17z=400.
 * https://oeis.org/A135636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) gives the x‑coordinate of the nth lexicographic positive
 *            integer solution (x,y,z) to 43*x + 7*y + 17*z = 400. The
 *            sequence is finite; for n beyond the number of solutions it
 *            returns 0
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t seq_cache[13];
static idx_t cached_count = 0;

static void build_cache(void)
{
    if (cached_count != 0)
        return;

    /* maximum possible x with y>=1,z>=1 */
    idx_t max_x = (400 - 7 - 17) / 43; /* integer division */

    idx_t pos = 0;
    for (idx_t x = 1; x <= max_x; ++x) {
        int R = 400 - 43 * (int)x;              /* remaining sum for 7y+17z */
        if (R < 24) /* need at least 7*1 + 17*1 = 24 */
            continue;

        idx_t y_max = (idx_t)(R - 17) / 7;     /* y such that z>=1 */
        for (idx_t y = 1; y <= y_max; ++y) {
            int val = R - 7 * (int)y;          /* 17*z */
            if (val % 17 == 0) {
                int z = val / 17;
                if (z >= 1) {                  /* z positive check */
                    seq_cache[pos++] = (seq_t)x;
                }
            }
        }
    }
    cached_count = pos;
}

seq_t a(idx_t n)
{
    build_cache();
    if (n > 0 && n <= cached_count)
        return seq_cache[n - 1];
    else
        return 0;
}
