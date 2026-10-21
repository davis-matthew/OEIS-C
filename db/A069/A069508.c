/*
 * A069508  a(1) = 6; a(n) = smallest palindromic number of the form k*a(n-1)
 *          + 1 with k > 1.
 * https://oeis.org/A069508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 6, a(1) = 55, a(n) = successive concatenation of digit 1
 *            of length n+1 for n ≥ 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;
    if (n == 1) return (seq_t)55;

    seq_t result = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        result = result * (seq_t)10 + (seq_t)1;
    }
    return result;
}
