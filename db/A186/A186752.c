/*
 * A186752  Length of minimum representation of the permutation [n,n-1,...,1]
 *          as the product of transpositions (1,2) and left and right
 *          rotations (1,2,...,n).
 * https://oeis.org/A186752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)/2 - 2 for n > 3, with initial values a(1)=0,
 *            a(2)=1, a(3)=2
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    seq_t nn = (seq_t)n;          /* cast once to seq_t */
    seq_t part = nn * (nn - (seq_t)1) / (seq_t)2;
    return part - (seq_t)2;
}
