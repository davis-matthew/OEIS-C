/*
 * A015012  Inverse of 1003rd cyclotomic polynomial.
 * https://oeis.org/A015012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse of the 1003rd cyclotomic polynomial.
   a(n) is the coefficient of x^n in the power‑series expansion of
   1/Φ₁₀₀₃(x).  The sequence is periodic with period 1003 and
   takes only the values 1, 0 and –1:
   • a(n) =  1   if (n mod 1003) ∈ [0,16]
   • a(n) =  0   if (n mod 1003) ∈ [17,58] or [76,1002]
   • a(n) = –1   if (n mod 1003) ∈ [59,75]  */

/* No additional includes are required; idx_t and seq_t are
   supplied by the build environment. */
#define PERIOD 1003u

seq_t a(idx_t n)
{
    /* Compute the residue modulo the period.  Use the unsigned
       modulo operation supplied by the compiler. */
    idx_t r = n % PERIOD;

    /* Return the periodic value according to the ranges above. */
    if (r <= 16u)
        return 1;
    if (r <= 58u)
        return 0;
    if (r <= 75u)
        return -1;
    return 0;
}
