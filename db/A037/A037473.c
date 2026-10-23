/*
 * A037473  Duplicate of A007092.
 * https://oeis.org/A037473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = interpret the base‑6 representation of n * as a base‑10
 *            number (digits 0–5 only)
 * Range:     i8 n <= 53, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;      /* the value to return                       */
    seq_t pow10   = 1;      /* 10^k for the current base‑6 digit position */
    idx_t m = n;           /* make a copy of n for the conversion loop  */

    /* Convert m to base‑6 and simultaneously build the decimal value */
    while (m > 0) {
        seq_t digit = (seq_t)(m % 6);   /* the least‑significant base‑6 digit */
        result += digit * pow10;        /* append the digit in decimal form */
        pow10 *= 10;                    /* move one decimal position left   */
        m /= 6;                         /* shift right one base‑6 digit     */
    }

    return result;
}
