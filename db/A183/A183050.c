/*
 * A183050  Sums of knight's moves to points as in A183049.
 * https://oeis.org/A183050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1..n} minKnightMoves(i, n-i)
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where minKnightMoves(x,y) is the minimum number of knight moves
   from (0,0) to (x,y) on an infinite board (x,y ≥ 0). */

#include <stdint.h>

static seq_t knight_moves(idx_t x, idx_t y)
{
    /* ensure x >= y */
    if (x < y) {
        idx_t tmp = x;
        x = y;
        y = tmp;
    }
    /* special small cases */
    if (x == 1 && y == 0) return (seq_t)3;
    if (x == 2 && y == 2) return (seq_t)4;
    /* general formula (e‑maxx) */
    seq_t d1 = ((seq_t)(x + 1)) >> 1;                     /* ceil(x/2) */
    seq_t d2 = ((seq_t)(x + y + 2)) / 3;                  /* ceil((x+y)/3) */
    seq_t d = d1 > d2 ? d1 : d2;
    /* adjust parity */
    if ( (d & 1) != (((seq_t)(x + y)) & 1) ) d++;
    return d;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        sum += knight_moves(i, n - i);
    }
    return sum;
}
