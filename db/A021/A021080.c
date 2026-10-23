/*
 * A021080  Decimal expansion of 1/76.
 * https://oeis.org/A021080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit at position n of decimal expansion of 1/76,
 *            computed by iterating the long‑division algorithm (remainder ×
 *            10 / 76)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t remainder = 1;
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        digit = (remainder * 10) / 76;
        remainder = (remainder * 10) % 76;
    }
    return digit;
}
