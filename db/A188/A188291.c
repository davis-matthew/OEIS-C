/*
 * A188291  [nr+kr]-[nr]-[kr], where r=sqrt(5), k=5, [ ]=floor.
 * https://oeis.org/A188291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n*sqrt(5)+5*sqrt(5)] - [n*sqrt(5)] - [5*sqrt(5)]
 * Range:     u8 none, u16 n <= 52, u32 n <= 132, u64 n <= 132, u128 n <= 132,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to the arbitrary‑precision type once */
    seq_t n_seq = (seq_t)n;

    /* Compute floor(n*sqrt(5)) as floor(sqrt(5*n^2)) */
    seq_t n_sq  = n_seq * n_seq;          /* n^2 */
    seq_t val1  = (seq_t)5 * n_sq;        /* 5*n^2 */
    seq_t a1    = floor_sqrt(val1);       /* floor(sqrt(5*n^2)) */

    /* Compute floor((n+5)*sqrt(5)) as floor(sqrt(5*(n+5)^2)) */
    seq_t m_seq = n_seq + (seq_t)5;       /* n+5 */
    seq_t m_sq  = m_seq * m_seq;          /* (n+5)^2 */
    seq_t val2  = (seq_t)5 * m_sq;        /* 5*(n+5)^2 */
    seq_t a2    = floor_sqrt(val2);       /* floor(sqrt(5*(n+5)^2)) */

    /* floor(5*sqrt(5)) is a constant 11 */
    const seq_t const_five = (seq_t)11;

    /* Difference gives the required term */
    return a2 - a1 - const_five;
}
