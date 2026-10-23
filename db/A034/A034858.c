/*
 * A034858  a(n) = C(n+3,4) + 3*C(n+1,3) + 5*C(n-1,2) + 7*n - 15.
 * https://oeis.org/A034858
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+3,4) + 3*C(n+1,3) + 5*C(n-1,2) + 7*n - 15
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 416, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 2  (a(2) is the first term) */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n < 2 the definition is not required, but we compute anyway */
    seq_t t1 = binomial(n + 3, 4);          /* C(n+3,4) */
    seq_t t2 = 3 * binomial(n + 1, 3);      /* 3*C(n+1,3) */
    seq_t t3 = 5 * binomial(n - 1, 2);      /* 5*C(n-1,2) */
    seq_t t4 = 7 * n;                       /* 7*n */
    seq_t result = t1 + t2 + t3 + t4 - 15;  /* combine all terms */
    return result;
}
