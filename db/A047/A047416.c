/*
 * A047416  Numbers that are congruent to {0, 1, 3, 4, 6} mod 8.
 * https://oeis.org/A047416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (8*(n-1))/5 )
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Computes the nth number congruent to {0,1,3,4,6} modulo 8,
       with a(1) = 0.  n is assumed >= 1. */
    seq_t m = (seq_t)n;   /* cast to the result type */
    m--;                  /* m = n-1 (n is guaranteed ≥ 1) */
    return m * 8 / 5;     /* integer division truncates toward zero */
}
