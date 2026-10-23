/*
 * A157445  a(n) = 14641*n^2 - 4598*n + 362.
 * https://oeis.org/A157445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14641*n^2 - 4598*n + 362
 * Range:     u8 none, u16 n <= 2, u32 n <= 541, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (seq_t)14641 * sn * sn - (seq_t)4598 * sn + (seq_t)362;
}
