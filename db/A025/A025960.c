/*
 * A025960  Expansion of 1/((1-2*x)*(1-4*x)*(1-5*x)*(1-8*x)).
 * https://oeis.org/A025960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8^(n+3)-8*5^(n+3)+9*4^(n+3)-2^(n+4))/72
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: a(0)=1, a(1)=19, a(2)=235,
   a(n) = (4^(n+1)-2^(n+1))/2 + 13*a(n-1) - 40*a(n-2)  for n>=2 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)19;

    /* Previous two terms: a_{k-2} and a_{k-1} */
    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)19; /* a(1) */

    /* Powers needed for k=2: 4^(k+1)=4^3, 2^(k+1)=2^3 */
    seq_t pow2 = pow_uint(2u, (idx_t)3); /* 2^3 = 8   */
    seq_t pow4 = pow_uint(4u, (idx_t)3); /* 4^3 = 64  */

    /* Compute terms up to n */
    for (idx_t k = 2; k <= n; ++k) {
        /* (4^(k+1)-2^(k+1))/2 */
        seq_t term = (pow4 - pow2) / (seq_t)2;
        /* Add recurrence components */
        term = term + (seq_t)13 * prev1 - (seq_t)40 * prev2;

        /* Advance for next iteration */
        prev2 = prev1;
        prev1 = term;
        pow2 *= (seq_t)2;   /* 2^(k+2) for next k */
        pow4 *= (seq_t)4;   /* 4^(k+2) for next k */
    }

    return prev1;
}
