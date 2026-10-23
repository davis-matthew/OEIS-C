/*
 * A014790  Inverse of 781st cyclotomic polynomial.
 * https://oeis.org/A014790
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

/* Inverse 781st cyclotomic polynomial:
   a(n) = 1 for n mod 781 ∈ [0,10],
          -1 for n mod 781 ∈ [71,81],
          0 otherwise.
   This follows from the recurrence
   a(k) = a(k-1) + a(k-781) - a(k-782) + c(k)
   with c(0)=1, c(11)=c(71)=-1, c(82)=1 and
   c(k)=0 for all other k; giving a periodic
   sequence of period 781. */

seq_t a(idx_t n)
{
    static const idx_t period = 781;
    idx_t r = n % period;
    if (r <= 10)
        return 1;
    if (r <= 70)
        return 0;
    if (r <= 81)
        return (seq_t)-1;
    return 0;
}
