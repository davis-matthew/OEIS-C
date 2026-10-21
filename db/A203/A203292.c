/*
 * A203292  Number of arrays of 4 nondecreasing integers in -n..n with sum
 *          zero and equal numbers greater than zero and less than zero.
 * https://oeis.org/A203292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (24 + 32*n + 6*n^2 + 4*n^3) / 24 for even n a(n) = (30 +
 *            32*n + 6*n^2 + 4*n^3) / 24 for odd n
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A203292, valid for all n >= 1. */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t num;

    if ((n & 1U) == 0)          /* even n */
    {
        num = (seq_t)24 + (seq_t)32 * sn + (seq_t)6 * sn * sn
              + (seq_t)4 * sn * sn * sn;
    }
    else                        /* odd n */
    {
        num = (seq_t)30 + (seq_t)32 * sn + (seq_t)6 * sn * sn
              + (seq_t)4 * sn * sn * sn;
    }

    return num / (seq_t)24;
}
