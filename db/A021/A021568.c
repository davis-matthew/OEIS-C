/*
 * A021568  Decimal expansion of 1/564.
 * https://oeis.org/A021568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit_{n+1} of the decimal expansion of 1/564, i.e. a(n)
 *            = floor( (10^(n+1)) / 564 ) mod 10, with a(0)=0
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* integer part of 0.001... */

    seq_t rem = (seq_t)1;          /* start with numerator 1 */
    idx_t steps = n + 1;           /* we need the (n+1)-th decimal digit */
    seq_t digit;

    for (idx_t i = 0; i < steps; ++i) {
        rem = rem * 10;            /* shift remainder left by one decimal place */
        digit = rem / 564;         /* current digit */
        rem = rem % 564;           /* remainder for next step */
    }

    return digit;                  /* last computed digit is the term */
}
