/*
 * A083078  6th row of number array A083075.
 * https://oeis.org/A083078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*13^n + 1)/6 for n ≥ 0 Recurrence: a(0) = 1, a(n) =
 *            13*a(n-1) - 2 (n > 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;               /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        val = val * 13 - 2;      /* a(n) = 13*a(n-1) - 2 */
    }
    return val;
}
