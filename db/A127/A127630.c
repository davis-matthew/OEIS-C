/*
 * A127630  Expansion of (1+x-x^2-x^3)/(1+x^2)^2.
 * https://oeis.org/A127630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n + 1 - (n mod 2))*(-1)^floor(n/2)
 * Range:     i8 n <= 127, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n >> 1;                 /* floor(n/2) */
    seq_t kval = (seq_t)k;            /* cast to seq_t for arithmetic */
    seq_t value = (seq_t)2 * kval + 1;
    seq_t sign  = ((k & 1) == 0) ? 1 : -1;
    return value * sign;
}
