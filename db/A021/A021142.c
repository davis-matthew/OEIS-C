/*
 * A021142  Decimal expansion of 1/138.
 * https://oeis.org/A021142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth decimal digit of 1/138 after the point (0‑Indexed)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) = floor(10^(n+1) / 138) mod 10  */
/*   Computed by long division:   */
/*   For i = 0..n: digit_i = floor(r*10 / 138),   r = r*10 mod 138;      */

seq_t a(idx_t n)
{
    /* remainder after the decimal expansion of 1/138 */
    seq_t r = (seq_t)1;          /* 1 % 138 == 1 */
    seq_t digit = (seq_t)0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t r10 = r * (seq_t)10;          /* r * 10 */
        digit = r10 / (seq_t)138;           /* next decimal digit */
        r = r10 % (seq_t)138;               /* update remainder */
    }
    return digit;
}
