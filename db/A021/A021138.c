/*
 * A021138  Decimal expansion of 1/134.
 * https://oeis.org/A021138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit n of the decimal expansion of 1/134 (0‑indexed)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
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
        remainder *= 10;
        digit = remainder / 134;
        remainder = remainder % 134;
    }
    return digit;
}
