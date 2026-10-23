/*
 * A188093  [nr+kr]-[nr]-[kr], where r=sqrt(3), k=6, [ ]=floor.
 * https://oeis.org/A188093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+6)*sqrt(3)) - floor(n*sqrt(3)) - 10, or
 *            equivalently a(n)=floor(n*sqrt(3)+6*sqrt(3)) - floor(n*sqrt(3))
 *            - 10
 * Range:     u8 none, u16 n <= 25, u32 n <= 143, u64 n <= 143, u128 n <= 143,
 *            big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the identity floor(n*sqrt(3)) = floor(sqrt(3*n*n)).
   The helper sqrt_floor from the utils library is employed to compute the integer
   square root of a seq_t value.  All arithmetic is performed in seq_t, so the
   function works for 8, 16, 32, 64, 128‐bit unsigned integers as well as
   arbitrary‑precision builds. */
#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    /* Compute floor(n*sqrt(3)) as floor(sqrt(3*n^2)) */
    seq_t n_seq = (seq_t)n;
    seq_t n_sq  = n_seq * n_seq;          /* n^2 */
    seq_t val1  = n_sq * ((seq_t)3);      /* 3*n^2 */
    seq_t floor1 = sqrt_floor(val1);      /* floor(n*sqrt(3)) */

    /* Compute floor((n+6)*sqrt(3)) as floor(sqrt(3*(n+6)^2)) */
    idx_t m = n + 6;
    seq_t m_seq = (seq_t)m;
    seq_t m_sq  = m_seq * m_seq;          /* (n+6)^2 */
    seq_t val2  = m_sq * ((seq_t)3);      /* 3*(n+6)^2 */
    seq_t floor2 = sqrt_floor(val2);      /* floor((n+6)*sqrt(3)) */

    /* Subtract as per the formula, using 10 = floor(6*sqrt(3)) */
    seq_t result = floor2 - floor1 - ((seq_t)10);
    return result;
}
