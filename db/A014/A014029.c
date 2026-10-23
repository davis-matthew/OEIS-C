/*
 * A014029  Inverse of 20th cyclotomic polynomial.
 * https://oeis.org/A014029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) - a(n-4) + a(n-6) - a(n-8) for n >= 8.
   Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=0, a(5)=0,
   a(6)=0, a(7)=0, a(8)=0, a(9)=0.  Period length 20. */
static const seq_t period[20] = {
    1,  0,  1,  0,  0,  0,  0,  0,  0,  0, -1,  0, -1,  0,  0,  0,
    0,  0,  0,  0
};

seq_t a(idx_t n)
{
    /* The sequence is periodic with period 20, so return the value
       corresponding to n modulo 20. */
    return period[n % 20];
}
