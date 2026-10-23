/*
 * A021556  Decimal expansion of 1/552.
 * https://oeis.org/A021556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/552) mod 10 (digit produced by successive
 *            long‑division of 1 by 552)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = 1;          /* current remainder before producing digit n */
    seq_t digit = 0;      /* value to return */

    for (idx_t i = 0; i <= n; ++i) {
        digit = (seq_t)(r * 10 / 552);
        r = r * 10 % 552;
    }
    return digit;
}
