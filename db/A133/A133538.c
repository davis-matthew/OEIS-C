/*
 * A133538  Sum of seventh powers of two consecutive primes.
 * https://oeis.org/A133538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n^7 + p_{n+1}^7, where p_k is the k-th prime
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 96, u128 n <= 4000,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k) */
#include "../utils/seq_pow.h"    /* seq_t seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* For safety, handle non‑positive n although the OEIS offset starts at 1. */
    if (n == 0) return 0;

    uint32_t p1 = get_prime(n);       /* the n‑th prime */
    uint32_t p2 = get_prime(n + 1);   /* the (n+1)‑th prime */

    seq_t v1 = seq_pow((seq_t)p1, 7); /* p1^7 */
    seq_t v2 = seq_pow((seq_t)p2, 7); /* p2^7 */

    return v1 + v2;                    /* sum of seventh powers */
}
