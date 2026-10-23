/*
 * A014027  Inverse of 18th cyclotomic polynomial.
 * https://oeis.org/A014027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n%18==0 or 3, -1 if n%18==9 or 12, 0 otherwise *
 *            (periodic of length 18, inverse of the 18th cyclotomic
 *            polynomial)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned r = (unsigned)(n % 18U);          /* compute n modulo 18   */
    switch (r) {
    case 0: case 3: return 1;                 /* a(0), a(3), a(18), …   */
    case 9: case 12: return -1;                /* a(9), a(12), a(27), …  */
    default: return 0;                        /* all other indices       */
    }
}
