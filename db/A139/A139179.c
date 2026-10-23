/*
 * A139179  Number of non-fourth-powers <= n.
 * https://oeis.org/A139179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - floor(n^(1/4))
 * Range:     u8 n <= 63, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t to avoid overflow when using sqrt */
    seq_t n_seq = (seq_t)n;

    /* First sqrt gives floor(sqrt(n)) */
    seq_t half = floor_sqrt(n_seq);

    /* Second sqrt gives floor(sqrt(floor(sqrt(n)))) = floor(n^(1/4)) */
    seq_t fourth = floor_sqrt(half);

    /* Subtract the count of fourth powers from n */
    return n_seq - fourth;
}
