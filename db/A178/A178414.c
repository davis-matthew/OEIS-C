/*
 * A178414  Least odd number in the Collatz (3x+1) preimage of odd numbers not
 *          a multiple of 3.
 * https://oeis.org/A178414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(3-(-1)^n)+1 Equivalently: a(n) = 2*n-1 if n is
 *            even a(n) = 4*n-3 if n is odd (valid for all n >= 1)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use parity of n to pick the appropriate formula. */
    if ((n & 1) == 0)          /* n is even */
        return ((seq_t)n * 2) - 1;
    else                         /* n is odd  */
        return ((seq_t)n * 4) - 3;
}
