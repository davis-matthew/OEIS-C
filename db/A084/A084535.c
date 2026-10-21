/*
 * A084535  a(n) = floor(n^2 - n^(3/2)).
 * https://oeis.org/A084535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2 - n^(3/2))
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t sq   = nn * nn;            /* n^2   */
    seq_t cube = nn * nn * nn;       /* n^3   */

    seq_t s   = floor_sqrt(cube);     /* floor(sqrt(n^3)) */
    seq_t sqs = s * s;                /* s^2   */

    seq_t ceil;
    if (sqs == cube)                  /* n^3 is a perfect square */
        ceil = s;
    else
        ceil = s + (seq_t)1;          /* ceil(sqrt(n^3)) */

    return sq - ceil;                 /* floor(n^2 - n^(3/2)) */
}
