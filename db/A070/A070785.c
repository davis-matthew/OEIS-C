/*
 * A070785  Square of shortest side of triangles with sides whose squares are
 *          integers and with positive integer area, ordered by longest side,
 *          then second longest side and finally shortest side.
 * https://oeis.org/A070785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function iterates over all triples (a,b,c) with 1 ≤ a ≤ b ≤ c.
   For each triple it checks the triangle inequality and
   the area condition  
       N = 2(ab+ac+bc) - (a²+b²+c²)  > 0
   Must satisfy N % 16 == 0 and N/16 is a perfect square.
   It returns the square of the shortest side (a) of the n‑th triangle
   in ascending order of (c, b, a). */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0, but never called that way */
    idx_t found = 0;

    for (idx_t c = 1; ; ++c)          /* longest side */
    {
        for (idx_t b = 1; b <= c; ++b)   /* second longest side */
        {
            for (idx_t a = 1; a <= b; ++a)   /* shortest side */
            {
                /* Triangle inequality: sqrt(a)+sqrt(b) > sqrt(c) */
                if (c > a + b)                /* diff >= 0  */
                {
                    unsigned __int128 diff = (unsigned __int128)c - a - b;
                    unsigned __int128 diff2 = diff * diff;
                    unsigned __int128 ab4  = (unsigned __int128)a * b * 4;
                    if (diff2 >= ab4) continue;
                }

                /* Area condition */
                seq_t as = (seq_t)a;
                seq_t bs = (seq_t)b;
                seq_t cs = (seq_t)c;

                seq_t ab = as * bs;
                seq_t ac = as * cs;
                seq_t bc = bs * cs;
                seq_t term = ((ab + ac + bc) << 1);            /* 2*(ab+ac+bc) */
                seq_t sub  = as * as + bs * bs + cs * cs;      /* a²+b²+c² */
                seq_t N    = term - sub;
                if (N <= 0)                 continue;
                if ((N & 15) != 0)          continue;           /* N % 16 != 0 */

                seq_t M = N >> 4;                    /* M = N/16 */
                seq_t r = floor_sqrt(M);
                if (r * r != M)                 continue;           /* not square */

                /* Valid triangle found */
                ++found;
                if (found == n) return as;   /* a is the shortest side squared */
            }
        }
    }
    return 0; /* unreachable */
}
