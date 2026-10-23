/*
 * A143244  a(n) = the smallest multiple of the n-th prime such that (a(n)-1)
 *          is divisible by both the (n-1)th prime and the (n+1)st prime.
 * https://oeis.org/A143244
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal multiple of p_n such that a(n)-1 ≡ 0 (mod
 *            p_{n-1}·p_{n+1})
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed as: let L = p_{n-1}*p_{n+1}, find inverse inv of p_n mod L, then a(n) = inv*p_n */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0; /* Undefined for n < 2, return 0 */

    /* primes surrounding the n-th prime */
    uint32_t p_prev = get_prime(n-1); /* (n-1)-th prime */
    uint32_t p_curr = get_prime(n);   /* n-th prime     */
    uint32_t p_next = get_prime(n+1); /* (n+1)-th prime */

    /* L = p_{n-1} * p_{n+1} */
    seq_t L = (seq_t)p_prev * (seq_t)p_next;

    /* Euler phi of L = (p_prev-1)*(p_next-1) */
    seq_t phiL = (seq_t)(p_prev-1) * (seq_t)(p_next-1);

    /* exponent for modular inverse: phi(L)-1 */
    seq_t exp_seq = phiL - (seq_t)1;
    idx_t exp = (idx_t)exp_seq; /* fits in 64 bits for n ≤ 10000 */

    /* Inverse of p_curr modulo L: p_curr^{phi(L)-1} mod L */
    seq_t inv = mod_pow((seq_t)p_curr, exp, L);

    /* Result */
    seq_t result = inv * (seq_t)p_curr;
    return result;
}
