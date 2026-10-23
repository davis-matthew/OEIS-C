/*
 * A021985  Decimal expansion of 1/981.
 * https://oeis.org/A021985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/981) mod 10
 * Range:     u8 n <= 3, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        r *= (seq_t)10;
        digit = r / (seq_t)981;
        r = r % (seq_t)981;
    }
    return digit;
}
