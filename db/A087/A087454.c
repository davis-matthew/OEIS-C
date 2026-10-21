/*
 * A087454  Multiplicative inverse of the n-th prime prime(n) modulo
 *          prime(n-1).
 * https://oeis.org/A087454
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = inverse_mod( prime(n), prime(n-1) ) (i.e., the
 *            multiplicative inverse of the n-th prime modulo the (n-1)-st
 *            prime)
 * Range:     u8 n <= 54, u16 n <= 6795, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helpers from the utilities */
#include "../utils/get_prime.h"   /* seq_t get_prime(idx_t k); */
#include "../utils/mod_pow.h"     /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

seq_t a(idx_t n)
{
    if (n < 2)                /* a(2) is the first defined term; below that we may return any value. */
        return (seq_t)0;

    /* Retrieve the consecutive primes. */
    seq_t p_prev = get_prime(n - 1);   /* prime(n‑1)   */
    seq_t p_curr = get_prime(n);       /* prime(n)     */

    /* Compute the inverse via Fermat's little theorem:
       p_curr^(p_prev-2) mod p_prev  */
    uint64_t exp = (uint64_t)(p_prev - (seq_t)2);
    return mod_pow(p_curr, exp, p_prev);
}
