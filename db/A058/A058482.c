/*
 * A058482  Number of 3 X n binary matrices with no zero rows or columns.
 * https://oeis.org/A058482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = 7^n - 3*3^n + 3 for n ≥ 1 For n ≤ 0, the function returns
 *            0 (the sequence is defined only for n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined, but return 0 for safety */

    /* Fast binary exponentiation that computes 7^n and 3^n in one pass. */
    seq_t pow7 = 1;                /* current power of 7   */
    seq_t pow3 = 1;                /* current power of 3   */
    seq_t base7 = 7;               /* base for 7  */
    seq_t base3 = 3;               /* base for 3  */
    idx_t exp = n;

    while (exp)
    {
        if (exp & 1)
        {
            pow7 *= base7;
            pow3 *= base3;
        }
        base7 *= base7;
        base3 *= base3;
        exp >>= 1;
    }

    seq_t result = pow7 - ((seq_t)3 * pow3) + (seq_t)3;
    return result;
}
