/*
 * A220944  Expansion of (1+3*x+5*x^2-x^3)/((1-x^2)*(1-3*x^2)).
 * https://oeis.org/A220944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 4*3^(n/2) - (n%2?1:3)
 *   a(2n)   = 4*3^n - 3
 *   a(2n+1) = 4*3^n - 1
 * The sequence is defined by the recurrence:
 *   a(0)=1, a(1)=3, a(n) = a(n-1)+2 if n odd,
 *   a(n) = a(n-1)*3 if n even.
 * This implementation uses the closed‑form expression with fast exponentiation.
 */

#include "../utils/big_pow.h"

/* Computes a(n) for A220944 */
seq_t a(idx_t n)
{
    /* k = floor(n/2) */
    idx_t k = n >> 1;       
    /* 3^k with arbitrary precision */
    seq_t pow3k = big_pow((seq_t)3, (seq_t)k); 
    /* 4*3^k */
    seq_t res   = (seq_t)4 * pow3k;             
    /* subtract the appropriate constant */
    if ((n & 1ULL) == 0)        /* even n: a(2k) = 4*3^k - 3 */
        res -= (seq_t)3;         
    else                        /* odd n:  a(2k+1) = 4*3^k - 1 */
        res -= (seq_t)1;         
    return res;
}
