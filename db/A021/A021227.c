/*
 * A021227  Decimal expansion of 1/223.
 * https://oeis.org/A021227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth decimal digit of 1/223 (the decimal expansion of
 *            1/223)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* We perform the classic long‑division algorithm for the fraction
       1/223.  A remainder r is maintained (initially 1).  For each
       iteration we multiply the remainder by 10; the quotient of that
       division by 223 is the next digit, and the new remainder is the
       remainder of that division.  After n+1 iterations the last
       computed digit is a(n).  All arithmetic is done in seq_t, so the
       code works for arbitrary‑precision builds as well. */
    seq_t r = 1;   /* Current remainder */
    seq_t digit = 0;   /* Digit to return */

    for (idx_t i = 0; i <= n; ++i) {
        r = r * 10;                               /* multiply by 10 */
        digit = r / 223;                           /* digit 0‑9     */
        r = r - digit * 223;                       /* new remainder */
    }
    return digit;
}
