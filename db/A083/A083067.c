/*
 * A083067  6th row of number array A083064.
 * https://oeis.org/A083067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*7^n + 1)/6 Equivalent recurrence: a(0) = 1, a(n) =
 *            7*a(n-1) - 1 for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value = 1;               /* a(0) */
    for (idx_t i = 1; i <= n; ++i) /* Compute up to a(n) */
    {
        value = value * (seq_t)7 - 1;
    }
    return value;
}
