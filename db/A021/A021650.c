/*
 * A021650  Decimal expansion of 1/646.
 * https://oeis.org/A021650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th digit of the decimal expansion of 1/646
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t remainder = 1;
    idx_t i;
    for (i = 0; i <= n; ++i) {
        remainder *= 10;
        seq_t digit = (seq_t)(remainder / 646);
        remainder %= 646;
        if (i == n)
            return digit;
    }
    return 0; /* unreachable */
}
