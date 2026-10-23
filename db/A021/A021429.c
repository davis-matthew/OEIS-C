/*
 * A021429  Decimal expansion of 1/425.
 * https://oeis.org/A021429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th digit (0‑based) of the decimal expansion of
 *            1/425
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Long division: start with remainder 1 (since 1/425 has numerator 1).
       At each step multiply the remainder by 10, the quotient is the next
       decimal digit, and the new remainder is the residue modulo 425. */
    idx_t r = 1;                 /* current remainder before each digit */
    seq_t digit = 0;             /* value of the digit just computed */

    /* We need n+1 iterations to produce digit a(n) (with a(0) being the
       first digit after the decimal point). */
    for (idx_t i = 0; i <= n; ++i)
    {
        r *= 10;                 /* shift to get the next digit */
        digit = (seq_t)(r / 425);/* integer division gives the digit 0‑9 */
        r %= 425;                /* keep the new remainder for the next step */
    }
    return digit;
}
