/*
 * A186150  Rank of (1/4)n^3 when {(1/4)i^3: i>=1} and {j^2>: j>=1} are
 *          jointly ranked with (1/4)i^3 after j^2 when (1/4)i^3=j^2.
 *          Complement of A186151.
 * https://oeis.org/A186150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n^(3/2))/2) + n which is equivalent to a(n) =
 *            floor_sqrt(n^3) / 2 + n because floor_sqrt(n^3) =
 *            floor(n^(3/2)). This gives the rank of n^3/4 in the joint list
 *            of (1/4)i^3 and j^2, with the cube after the square when equal
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>             /* for uint64_t, etc. */
#include "../utils/floor_sqrt.h" /* provides seq_t floor_sqrt(seq_t x); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n = 0, but safeguard */
    seq_t nn = (seq_t)n;
    /* compute n^3 safely within seq_t */
    seq_t cube = nn * nn * nn;        /* n^3 */
    seq_t sq   = floor_sqrt(cube);    /* floor(sqrt(n^3)) = floor(n^(3/2)) */
    seq_t part = sq / (seq_t)2;       /* floor(sqrt(n^3))/2 = floor(n^(3/2)/2) */
    return part + nn;                 /* rank = floor(n^(3/2)/2) + n */
}
