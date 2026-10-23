/*
 * A108903  Numbers such that the outer 2 digits are 9 and the inner digits
 *          are 5.
 * https://oeis.org/A108903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 10 * a(n-1) - 31, a(1) = 959
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1; n>=1 for valid terms */
    seq_t result = (seq_t)959;   /* a(1) */
    for (idx_t i = 1; i < n; ++i)   /* apply recurrence n-1 times */
    {
        result = result * 10 - 31;
    }
    return result;
}
