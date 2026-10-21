/*
 * A060586  Minimum number of moves required to reach position n starting from
 *          0 in Tower of Hanoi (with three pegs: 0,1,2), where with position
 *          n written in base 3, xyz means smallest disk is on peg z, second
 *          smallest is on peg y, third smallest on peg x, etc. and leading
 *          zeros indicate largest disks are al...
 * https://oeis.org/A060586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nice, nonn
 * Formula:   a(n) = A060585(A060583(n))
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   moves_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/moves_rec.h"
/* Recursive computation of the minimum number of moves in the 3‑peg Tower of Hanoi.
 * seq_t and idx_t are provided by the build system.  No external helpers are used.
 */
static int num_digits(idx_t n)
{
    if (n == 0)
        return 0;
    int cnt = 0;
    do {
        ++cnt;
        n /= 3;
    } while (n > 0);
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    int nd = num_digits(n);
    return moves_rec(n, 0, nd);
}
