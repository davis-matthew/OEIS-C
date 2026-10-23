/*
 * A014020  Inverse of 11th cyclotomic polynomial.
 * https://oeis.org/A014020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (11*m^10 - 595*m^9 + 13980*m^8 - 186990*m^7 + 1566663*m^6
 *            * - 8513715*m^5 + 29974570*m^4 - 65946860*m^3 + 82751976*m^2 * -
 *            46916640*m + 3628800)/3628800, where m = n mod 11. * Periodic
 *            with period 11
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed periodic values for m = n mod 11. */
    static const seq_t table[11] = {
        (seq_t)1, (seq_t)-1, (seq_t)0, (seq_t)0, (seq_t)0,
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0
    };
    idx_t m = n % 11;
    return table[m];
}
