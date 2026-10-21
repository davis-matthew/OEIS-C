/*
 * A083818  Numbers k such that 2k-1 is the digit reversal of k.
 * https://oeis.org/A083818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, changed
 * Formula:   a(n) = 4*10^(n-1) - 3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = 1 + 36*A000042(n-1) or the recurrence
   a(n) = 11*a(n-1) - 10*a(n-2) for n >= 3, with a(1)=1, a(2)=37. */
seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 according to OEIS offset */
    seq_t power_of_ten = 1;
    /* compute 10^(n-1) */
    for (idx_t i = 1; i < n; ++i)
        power_of_ten *= 10;

    /* a(n) = 4*10^(n-1) - 3 */
    return power_of_ten * (seq_t)4 - (seq_t)3;
}
