/*
 * A166872  a(n) = floor(n/2 + 2 - sqrt(17/4 + 2*n)).
 * https://oeis.org/A166872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/2 + 2 - sqrt(17/4 + 2*n))
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute v = 17 + 8*n */
    seq_t N = (seq_t)n;
    seq_t v = ((seq_t)17) + ((seq_t)8) * N;

    /* floor sqrt of v */
    seq_t s = floor_sqrt(v);

    /* compute ceil sqrt */
    seq_t ceil_s = (s * s == v) ? s : s + ((seq_t)1);

    /* result = (n + 4 - ceil_s) / 2 */
    seq_t tmp = N + ((seq_t)4) - ceil_s;
    return tmp / ((seq_t)2);
}
