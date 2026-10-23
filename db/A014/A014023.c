/*
 * A014023  Inverse of 14th cyclotomic polynomial.
 * https://oeis.org/A014023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A014023
   Formula: periodic with period 14
   a(n) =  1  for n mod 14 = 0,1
   a(n) = -1  for n mod 14 = 7,8
   a(n) =  0  otherwise
   This implements the generating function 1/(1-x+x^2-x^3+x^4-x^5+x^6). */
seq_t a(idx_t n)
{
    /* compute n modulo 14 using only integer arithmetic */
    uint64_t r = n % 14;
    /* branchless implementation using switch for speed */
    switch ((int)r) {
        case 0: case 1:
            return 1;               /* a(0) = a(1) = 1 */
        case 7: case 8:
            return -1;              /* a(7) = a(8) = -1 */
        default:
            return 0;               /* all other positions */
    }
}
