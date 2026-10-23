/*
 * A047030  Array T read by diagonals: T(h,k)=number of paths consisting of
 *          steps from (0,0) to (h,k) such that each step has length 1
 *          directed up or right and no step touches the line y=x/4 unless x=0
 *          or x=h.
 * https://oeis.org/A047030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of lattice paths from (0,0) to (h,k) that use only
 *            unit steps right or up, such that no step touches the line y =
 *            x/4 unless the step lies at x = 0 (the start) or x = h (the
 *            end). The pair (h,k) is derived from n by reading array T along
 *            its diagonals: let d be the unique integer wi...
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

/* maximum coordinate needed for n up to about 10 000;
   200 is more than enough for the required range. */
#define MAX_COORD 200

static seq_t dp[MAX_COORD + 1][MAX_COORD + 1];
static bool built = false;

seq_t a(idx_t n)
{
    if (!built) {
        /* initialise DP table */
        for (idx_t x = 0; x <= MAX_COORD; ++x)
            for (idx_t y = 0; y <= MAX_COORD; ++y)
                dp[x][y] = 0;

        dp[0][0] = (seq_t)1;

        for (idx_t x = 0; x <= MAX_COORD; ++x) {
            for (idx_t y = 0; y <= MAX_COORD; ++y) {
                if (x == 0 && y == 0)
                    continue;

                seq_t val = 0;

                /* contribution from left neighbour (x-1, y) via a right step */
                if (x > 0) {
                    idx_t lx = x - 1, ly = y;
                    /* the left node is allowed to leave only if it is not
                       on the forbidden line (except for the origin). */
                    bool left_allowed = !(((lx != 0) || (ly != 0)) && (4 * ly == lx));
                    if (left_allowed)
                        val += dp[lx][ly];
                }

                /* contribution from below neighbour (x, y-1) via an up step */
                if (y > 0) {
                    idx_t bx = x, by = y - 1;
                    /* starting node must not be on the forbidden line
                       (except for the origin). */
                    bool start_allowed = !(((bx != 0) || (by != 0)) && (4 * by == bx));
                    if (start_allowed) {
                        /* also the step must not cross the line interiorly.
                           A vertical step from (x,by) to (x,by+1) crosses the
                           line if x ∈ [4*by+1 , 4*by+3]. */
                        if (!(x >= 4 * by + 1 && x <= 4 * by + 3))
                            val += dp[bx][by];
                    }
                }

                dp[x][y] = val;
            }
        }
        built = true;
    }

    /* Compute the diagonal index d such that d(d+1)/2 ≤ n < (d+1)(d+2)/2 */
    idx_t d = (idx_t)((sqrt((double)(8 * n + 1)) - 1) / 2);
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;
    while (d * (d + 1) / 2 > n)
        --d;

    idx_t offset = n - d * (d + 1) / 2;
    idx_t h = offset;          /* row index starting at 0 */
    idx_t k = d - offset;      /* column index */

    if (h > MAX_COORD || k > MAX_COORD)
        return (seq_t)0;       /* safety guard for unexpected large n */

    return dp[h][k];
}
