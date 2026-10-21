/*
 * A051580  a(n) = (2*n+6)!!/6!!, related to A000165 (even double factorials).
 * https://oeis.org/A051580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+6)!!/6!! (even double factorial) Recurrence: a(0) =
 *            1; a(n+1) = (2*n+8) * a(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        result *= (seq_t)(2 * i + 8);
    }
    return result;
}
