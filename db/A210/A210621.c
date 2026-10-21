/*
 * A210621  Decimal expansion of 256/81.
 * https://oeis.org/A210621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th decimal digit of the expansion of 256/81 (OEIS
 *            A210621). The integer part (3) is a(1); the following digits are
 *            produced by long division of the remainder 13 by 81
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term (1‑indexed).  Returns 0 for n == 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 3;

    idx_t rem = 13;           /* remainder after the integer part 3 */
    seq_t digit = 0;

    for (idx_t i = 2; i <= n; ++i) {
        digit = (seq_t)(rem * 10 / 81);
        rem   = rem * 10 % 81;
        if (i == n) return digit;
    }
    return digit; /* unreachable, keeps compiler happy */
}
