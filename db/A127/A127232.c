/*
 * A127232  Eulerian numbers A000295 appearing in the intersection of
 *          Fibonacci and Lucas sequences A124168 and A127208.
 * https://oeis.org/A127232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(( (n+2)*2^(n-1) - 1 )) - (n+2)*2^(n-1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;  /* safety for n < 1 */

    seq_t two = (seq_t)2;
    seq_t exp1 = (seq_t)(n - (idx_t)1);          /* exponent n-1 */
    seq_t p = big_pow(two, exp1);                /* p = 2^(n-1) */
    seq_t k = p * ((seq_t)(n + (idx_t)2));       /* k = (n+2)*2^(n-1) */
    seq_t exp2 = k - (seq_t)1;                   /* exponent k-1   */
    seq_t pow_k = big_pow(two, exp2);            /* 2^(k-1)        */
    return pow_k - k;
}
