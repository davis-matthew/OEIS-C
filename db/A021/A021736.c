/*
 * A021736  Decimal expansion of 1/732.
 * https://oeis.org/A021736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/732) mod 10
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t remainder = (seq_t)1;
    seq_t digit = (seq_t)0;
    for (idx_t i = 0; i <= n; ++i) {
        remainder = remainder * (seq_t)10;
        digit = remainder / (seq_t)732;
        remainder = remainder % (seq_t)732;
    }
    return digit;
}
