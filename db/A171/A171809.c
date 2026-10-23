/*
 * A171809  Number of 5 X 5 permutation matrices such that the n-th matrix
 *          power is the identity matrix and the determinant is -1.
 * https://oeis.org/A171809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   a(n) = number of odd permutations in S5 whose order is exactly
 *            n. Only nonzero values: a(2)=10, a(4)=30, a(6)=20, all other n
 *            >= 1 give 0
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)2)
        return (seq_t)10;
    if (n == (idx_t)4)
        return (seq_t)30;
    if (n == (idx_t)6)
        return (seq_t)20;
    return (seq_t)0;
}
