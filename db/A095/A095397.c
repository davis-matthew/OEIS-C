/*
 * A095397  Modified juggler map: see A095396. Largest value in trajectory of
 *          started n under the juggler map of A095396.
 * https://oeis.org/A095397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 24, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   big_pow, floor_cube_root, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Modified juggler map:
   a(n) = largest value in the trajectory of started n under the
   juggler map of A095396: odd → floor(sqrt(n^3)), even → floor((n^2)^(1/3)). */
#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t max = cur;

    while (cur != (seq_t)1)
    {
        seq_t next;
        if ((cur & (seq_t)1) == (seq_t)0)      /* even */
        {
            seq_t sq = big_pow(cur, (seq_t)2);
            next = floor_cube_root(sq);
        }
        else                                   /* odd */
        {
            seq_t cube = big_pow(cur, (seq_t)3);
            next = floor_sqrt(cube);
        }
        cur = next;
        if (cur > max)
            max = cur;
    }
    return max;
}
