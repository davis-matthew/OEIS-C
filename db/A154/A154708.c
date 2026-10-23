/*
 * A154708  Numbers a such that b and c exist with b <= a < c and a*(a+1) +
 *          b^2 = c^2.
 * https://oeis.org/A154708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + (n mod 2) (valid for n >= 1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against the undefined case n == 0, though the sequence
       definition starts at n = 1. Return 0 for safety. */
    if (n == 0)
        return 0;
    return (seq_t)n * 2 + (seq_t)(n & (idx_t)1);
}
