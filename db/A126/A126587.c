/*
 * A126587  a(n) is the number of integer lattice points inside the right
 *          triangle with legs 3n and 4n (and hypotenuse 5n).
 * https://oeis.org/A126587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^2 - 4*n + 1
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 6 * nn * nn - 4 * nn + 1;
}
