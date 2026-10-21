/*
 * A060084  a(n) is the least prime not a primitive root of n-th prime.
 * https://oeis.org/A060084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 6675, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize, get_prime, is_prime_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A060084: a(n) = the least prime that is NOT a primitive root
 * of the n‑th prime (index 1‑based). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"              /* uint32_t get_prime(idx_t k);    */
#include "../utils/is_prime_small.h"         /* bool is_prime_small(uint64_t n);*/
#include "../utils/factorize.h"             /* size_t factorize(uint64_t n,
                                              uint64_t primes[], int exps[]); */
#include "../utils/mod_pow.h"               /* seq_t mod_pow(seq_t base,
                                              uint64_t exp, seq_t mod); */

seq_t a(idx_t n)
{
    /* The n‑th prime (1‑based). */
    uint32_t q_u32 = get_prime(n);
    uint64_t q = (uint64_t)q_u32;              /* promote for arithmetic */

    /* Handle the trivial base case q = 2 directly. */
    if (q == 2)
        return (seq_t)2;

    /* Factor q-1. */
    uint64_t fac[64];          /* prime factors of q-1 */
    int      exp[64];          /* their exponents (unused) */
    size_t   cnt = factorize(q - 1, fac, exp);

    /* Search the least prime which is not a primitive root modulo q. */
    for (uint64_t p = 2; ; ++p) {
        if (!is_prime_small(p))
            continue;              /* skip composites */

        /* If p == q (i.e., p mod q == 0), it cannot be a primitive root. */
        if (p % q == 0)
            return (seq_t)p;

        bool ok = true;
        for (size_t i = 0; i < cnt && ok; ++i) {
            uint64_t exp_val = (q - 1) / fac[i];
            seq_t   v = mod_pow((seq_t)p, exp_val, (seq_t)q);
            if (v == (seq_t)1)           /* violates primitive root test */
                ok = false;
        }
        if (!ok)
            return (seq_t)p;
    }
}
