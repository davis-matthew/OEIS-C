/*
 * A083597  a(n) = (7*4^n - 4)/3.
 * https://oeis.org/A083597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (7*4^n - 4)/3 Recurrence: a(0) = 1, a(n+1) = 4*a(n) + 4
 *            (n ≥ 0)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;          /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        val = (val * 4) + 4;   /* a(i+1) = 4*a(i) + 4 */
    }
    return val;
}
