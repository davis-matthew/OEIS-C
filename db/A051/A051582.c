/*
 * A051582  a(n) = (2*n+8)!!/8!!, related to A000165 (even double factorials).
 * https://oeis.org/A051582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(n) = a(n-1) * (2*n + 8) for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        /* current multiplier is (2*(i) + 10) == (2*(n-1) + 10) when loop reads i = n-1 */
        seq_t mul = (seq_t)(2 * i + 10);
        res *= mul;
    }
    return res;
}
