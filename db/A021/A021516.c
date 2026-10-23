/*
 * A021516  Decimal expansion of 1/512.
 * https://oeis.org/A021516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The decimal expansion of 1/512: a(n) is the nth digit after the
   decimal point in the decimal representation of 1/512.
   a(0)=0, a(1)=0, a(2)=1, a(3)=9, a(4)=5, ... */
seq_t a(idx_t n)
{
    /* Rational 1/512; we perform long division to extract the nth digit. */
    seq_t remainder = 1;           /* start with numerator 1 */
    seq_t denom = 512;             /* denominator */
    idx_t i;

    for (i = 0; i <= n; ++i) {
        remainder *= 10;          /* bring down next decimal digit */
        seq_t digit = remainder / denom;
        remainder = remainder % denom;
        if (i == n) return digit;
    }
    return 0; /* unreachable, but keeps compiler happy */
}
