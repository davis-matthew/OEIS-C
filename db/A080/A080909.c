/*
 * A080909  a(n) = (2n+1)! modulo 4n+3, |a(n)| <= 1.
 * https://oeis.org/A080909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2*n+1)! modulo (4*n+3) (|a(n)| ≤ 1)
 * Range:     i8 n <= 31, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is 0 when 4*n+3 is composite, otherwise it is ±1.  
   The computation uses the fact that for a prime p=4*n+3 we have  
   a(n) = ((p-1)/2)! mod p, which is always ±1.                */

#include <stdbool.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    /* 4*n+3 fits in idx_t; use it for primality testing. */
    idx_t m_idx = 4*n + 3;

    /* Composite modulus ⇒ factorial contains the modulus. */
    if (!isPrime(m_idx))
        return (seq_t)0;

    /* Work modulo m in seq_t arithmetic. */
    seq_t m = (seq_t)m_idx;
    /* (p-1)/2 = 2*n+1 = (m-1)/2 = half. */
    idx_t half = (m_idx - 1) / 2;

    seq_t prod = 1;
    for (idx_t i = 1; i <= half; ++i)
        prod = (prod * (seq_t)i) % m;

    /* prod is either 1 or m-1 (≡ -1). */
    if (prod == m - 1)
        return (seq_t)-1;
    return prod;   /* == 1 */
}
