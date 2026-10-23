/*
 * A021532  Decimal expansion of 1/528.
 * https://oeis.org/A021532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th digit of the decimal expansion of 1/528
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The digit is obtained by performing long division: start with
   remainder 1, repeatedly multiply by 10, and take the quotient
   mod 10.  The loop runs exactly n+1 times. */
seq_t a(idx_t n)
{
    const seq_t denom = (seq_t)528;
    const seq_t ten   = (seq_t)10;
    seq_t r = (seq_t)1;          /* initial remainder */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        r = r * ten;            /* bring down next digit */
        digit = r / denom;      /* integer part is the digit */
        r = r % denom;          /* remainder for next step */
    }
    return (seq_t)digit;
}
