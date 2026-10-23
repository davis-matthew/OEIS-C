/*
 * A014031  Inverse of 22nd cyclotomic polynomial.
 * https://oeis.org/A014031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 none, i16 none, i32 n <= 1000, i64 n <= 1000, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A014031: Inverse of the 22nd cyclotomic polynomial
   Formula: a(n) = a(n-22).  For all n,
   let m = n mod 11,  s = (-1)^(floor(n/11)).  Then
      a(n) = ( (-9*m^10 + 485*m^9 - 11340*m^8 + 150690*m^7
                - 1251117*m^6 + 6709605*m^5 - 23140710*m^4
                + 49127860*m^3 - 57244824*m^2 + 25659360*m + 3628800)
              * s ) / 3628800
   The division is exact for all integer m. */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m   = n % 11;                    /* 0 <= m <= 10 */
    seq_t s   = ((n / 11) & 1) ? -1 : 1;    /* sign */

    seq_t t = -9;
    t = t * (seq_t)m + 485;
    t = t * (seq_t)m - 11340;
    t = t * (seq_t)m + 150690;
    t = t * (seq_t)m - 1251117;
    t = t * (seq_t)m + 6709605;
    t = t * (seq_t)m - 23140710;
    t = t * (seq_t)m + 49127860;
    t = t * (seq_t)m - 57244824;
    t = t * (seq_t)m + 25659360;
    t = t * (seq_t)m + 3628800;

    t = t * s;           /* apply the alternating sign */
    return t / 3628800;  /* exact division */
}
