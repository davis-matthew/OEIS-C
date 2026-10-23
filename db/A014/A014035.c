/*
 * A014035  Inverse of 26th cyclotomic polynomial.
 * https://oeis.org/A014035
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

/* Recurrence for A014035: a(0)=1, a(1)=1, a(13)=a(14)=-1, all other
   a(n)=0 for 0≤n<26.  The sequence is periodic with period 26. 
   Formula used: a(n) = 1 if n mod 26 ∈ {0,1}, a(n) = –1 if n mod 26 ∈ {13,14},
   otherwise a(n) = 0. */
seq_t a(idx_t n)
{
    idx_t q = n % 26ull;
    if (q == 0 || q == 1) return (seq_t)1;
    if (q == 13 || q == 14) return (seq_t)-1;
    return (seq_t)0;
}
