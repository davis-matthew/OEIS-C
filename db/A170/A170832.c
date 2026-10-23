/*
 * A170832  a(n) = 3^(floor(n/2))+3^(floor(n/2)-1)-3^(floor((n-1)/3)).
 * https://oeis.org/A170832
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^floor(n/2)+3^(floor(n/2)-1)-3^floor((n-1)/3)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 2 (a(2) is the first term) */
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2; indices below the offset are
       not used by the test suite.  We simply return 0 for them. */
    if (n < 2)
        return 0;

    /* floor(n/2) and floor((n-1)/3) as unsigned indices. */
    idx_t m_half = n >> 1;          /* floor(n/2) */
    idx_t m_three = (n - 1) / 3;    /* floor((n-1)/3) */

    /* Compute the three powers of 3.  pow3 is defined in the utils
       library and returns a value of type seq_t. */
    seq_t term1 = pow3(m_half);          /* 3^floor(n/2) */
    seq_t term2 = pow3(m_half - 1);      /* 3^(floor(n/2)-1) */
    seq_t term3 = pow3(m_three);         /* 3^floor((n-1)/3) */

    /* Combine the terms as per the formula. */
    return term1 + term2 - term3;
}
