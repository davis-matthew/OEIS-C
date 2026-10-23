/*
 * A161462  Final digit of the sum of all the integers from n to 2*n-1.
 * https://oeis.org/A161462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ((3*n - 1)*n/2) mod 10
 * Range:     u8 n <= 9, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the last digit of the sum of integers from n to 2*n-1. */
seq_t a(idx_t n)
{
    seq_t nn = n;
    /* (3*n - 1)*n is always even, safe to divide by 2. */
    seq_t term = ((seq_t)3 * nn - (seq_t)1) * nn;
    term /= (seq_t)2;
    return term % (seq_t)10;
}
