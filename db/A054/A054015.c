/*
 * A054015  a(n) is Chowla function of n read modulo (number of proper
 *          divisors of n), a(1) = 0 by convention.
 * https://oeis.org/A054015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Chowla(n) mod proper_divisors(n)) where Chowla(n) =
 *            sigma(n) – 1 – n and proper_divisors(n) = divisor_count(n) – 1
 *            a(1) = 0 by convention
 * Range:     u8 n <= 3881, u16 n <= 9920, u32 n <= 9920, u64 n <= 9920,
 *            u128 n <= 9920, big any n
 * Helpers:   divisor_count_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)                /* a(1) = 0 */
        return 0;

    /* divisor count, proper divisor count, and modulus */
    idx_t d = divisor_count_u64((uint64_t)n);
    idx_t proper_cnt = d - 1;          /* >= 1 for n > 1 */
    if (proper_cnt <= 1)               /* n is prime → result 0 */
        return 0;

    seq_t mod = (seq_t)proper_cnt;     /* modulus as seq_t */

    /* compute sigma(n) modulo mod using the prime factorisation of n */
    seq_t sigma_mod = 1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);  /* prime factors of n */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p    = (seq_t)primes[i];
        seq_t term = 1;            /* sum of powers of p (including p^0) */
        seq_t pow  = 1;            /* current p^k */

        for (uint64_t k = 1; k <= exps[i]; ++k) {
            pow  = (pow * p) % mod;     /* p^k (mod mod) */
            term = (term + pow) % mod;  /* add to sum */
        }

        sigma_mod = (sigma_mod * term) % mod; /* accumulate product */
    }

    /* Now compute (sigma_mod – 1 – n) modulo mod safely */
    seq_t n_mod = (seq_t)(n % proper_cnt);
    seq_t tmp   = sigma_mod + mod - 1 - n_mod;   /* non‑negative */
    seq_t result = tmp % mod;

    return result;
}
