/*
 * A183047  Sums of least knight's moves from (0,0) to points in the square
 *          lattice [-n,n]x[-n,n].
 * https://oeis.org/A183047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{(x,y)∈[−n,n]²} d(x,y) where d is the * minimal number
 *            of knight moves from (0,0) to (x,y). The * function uses the
 *            known closed‐form formula for knight distance * on an infinite
 *            board (with a few special cases). No lookup tables * are used.
 *            The result fits in seq_t, assuming the caller...
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the minimal knight distance to (x,y) on an infinite board. */
static seq_t knight_dist(idx_t x, idx_t y)
{
    idx_t a = x, b = y;
    if (a < b) { idx_t t = a; a = b; b = t; }   /* a ≥ b ≥ 0 */

    /* Special cases */
    if (a == 0 && b == 0) return 0;
    if (a == 1 && b == 0) return 3;   /* (±1,0) or (0,±1) */
    if (a == 1 && b == 1) return 2;   /* (±1,±1) */
    if (a == 2 && b == 0) return 2;   /* (±2,0) */
    if (a == 2 && b == 1) return 1;   /* (±2,±1) */
    if (a == 2 && b == 2) return 4;   /* special, otherwise formula gives 2 */

    /* General closed‑form (see https://oeis.org/A001105) */
    idx_t d1 = (a + 1) / 2;                   /* ceil(a/2)        */
    idx_t d2 = (a + b + 2) / 3;               /* ceil((a+b)/3)    */
    idx_t d  = (d1 > d2) ? d1 : d2;           /* max(d1,d2)       */

    /* Parity adjustment: distance and (a+b) must have same parity */
    if ((d & 1) != ((a + b) & 1)) d++;

    return (seq_t)d;
}

/* Sum of minimal knight moves to all points in [−n,n]² */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t x = 0; x <= n; ++x) {
        for (idx_t y = 0; y <= n; ++y) {
            seq_t d = knight_dist(x, y);

            if (x == 0 && y == 0) {
                sum += d;               /* origin – once */
            } else if (x == 0 || y == 0) {
                sum += d * 2;          /* axis points – two symmetries */
            } else {
                sum += d * 4;          /* interior points – four quadrants */
            }
        }
    }

    return sum;
}
