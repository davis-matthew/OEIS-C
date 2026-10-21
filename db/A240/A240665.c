/*
 * A240665  Least k such that 10^k == -1 (mod prime(n)), or 0 if no such k
 *          exists.
 * https://oeis.org/A240665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that 10^k ≡ -1 (mod prime(n)); 0 if none. *
 *            The algorithm: compute the multiplicative order O of 10 modulo
 *            the * n‑th prime p. If O is even, a(n) = O/2 (otherwise 0). The
 *            order is * obtained by repeatedly dividing O by each distinct
 *            prime factor of p-1 * while 10^(O/q) ≡ 1 (mo...
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* returns uint32_t get_prime(idx_t k) */
#include "../utils/mod_pow.h"     /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */

seq_t a(idx_t n) {
    /* Retrieve the n-th prime (1‑based). */
    uint32_t p_val = get_prime(n);
    if (p_val == 2 || p_val == 5)           /* 10 not invertible mod 2 or 5 */
        return (seq_t)0;

    seq_t p = (seq_t)p_val;                  /* prime as seq_t */
    uint64_t m = (uint64_t)p_val - 1ULL;     /* p-1 fits in 64 bits for our limits */

    /* Factor m into distinct prime factors. */
    uint64_t factors[64];
    size_t fc = 0;
    uint64_t tmp = m;

    if (tmp % 2ULL == 0ULL) {
        factors[fc++] = 2ULL;
        while (tmp % 2ULL == 0ULL) tmp /= 2ULL;
    }
    for (uint64_t d = 3ULL; d * d <= tmp; d += 2ULL) {
        if (tmp % d == 0ULL) {
            factors[fc++] = d;
            while (tmp % d == 0ULL) tmp /= d;
        }
    }
    if (tmp > 1ULL)
        factors[fc++] = tmp;                 /* remaining prime factor */

    /* Compute the multiplicative order of 10 modulo p. */
    uint64_t order = m;
    for (size_t i = 0; i < fc; ++i) {
        uint64_t q = factors[i];
        while (order % q == 0ULL) {
            uint64_t cand = order / q;
            seq_t r = mod_pow((seq_t)10, cand, p);
            if (r == 1) {
                order = cand;
            } else {
                break;
            }
        }
    }

    /* If the order is odd, -1 cannot appear as a power of 10. */
    if (order % 2ULL != 0ULL)
        return (seq_t)0;

    /* Verify that the element of order 2 is indeed -1. */
    seq_t r = mod_pow((seq_t)10, order / 2ULL, p);
    if (r == p - 1)
        return (seq_t)(order / 2ULL);
    else
        return (seq_t)0;
}
