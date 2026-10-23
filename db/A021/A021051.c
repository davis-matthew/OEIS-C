/*
 * A021051  Decimal expansion of 1/47.
 * https://oeis.org/A021051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A021051 – decimal expansion of 1/47. */
/* a(0) = 0 (the integer part). For n ≥ 1, a(n) is the nth decimal digit after
   the initial 0 that follows the decimal point. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    const seq_t denom = (seq_t)47;               /* modulus for the division */
    seq_t r = (seq_t)10 % denom;                /* remainder after the leading 0 */
    seq_t digit = 0;

    for (idx_t i = 1; i <= n; ++i) {
        digit = (r * 10) / denom;
        r = (r * 10) % denom;
    }
    return digit;
}
