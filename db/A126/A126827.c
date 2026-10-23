/*
 * A126827  Ramanujan numbers (A000594) read mod 27.
 * https://oeis.org/A126827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 * sigma_7(n)) mod 27, where sigma_k is the divisor
 *            sum function. This computes sigma_7(n) modulo 27 by prime
 *            factorisation and modular arithmetic. Only integer operations
 *            are used
 * Range:     u8 n <= 1428, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp modulo mod using binary exponentiation.
   All intermediate products are performed in uint64_t to prevent
   overflow when seq_t is smaller than 64 bits. */
static inline seq_t mod_pow_seq(seq_t base, uint64_t exp, seq_t mod)
{
    uint64_t res  = 1 % (uint64_t)mod;
    uint64_t b    = (uint64_t)base % (uint64_t)mod;
    while (exp) {
        if (exp & 1)
            res = (res * b) % (uint64_t)mod;
        b = (b * b) % (uint64_t)mod;
        exp >>= 1;
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    /* Modulus */
    const seq_t MOD = 27;

    /* n^2 mod 27 */
    uint64_t n_mod64 = (uint64_t)n % 27;
    seq_t n2_mod = (seq_t)((n_mod64 * n_mod64) % 27);

    /* Compute sigma_7(n) mod 27 by prime factorisation */
    seq_t sigma_mod = 1;      /* multiplicative identity */

    idx_t temp = n;
    if (temp % 2 == 0) {
        int e = 0;
        seq_t p_mod = ((seq_t)2) % MOD;
        seq_t p7_mod = mod_pow_seq(p_mod, 7, MOD);
        while (temp % 2 == 0) {
            ++e;
            temp /= 2;
        }
        seq_t sum   = 1;
        seq_t cur   = 1;
        for (int i = 0; i < e; ++i) {
            cur = (uint64_t)cur * p7_mod % 27;
            sum = (sum + cur) % 27;
        }
        sigma_mod = (uint64_t)sigma_mod * sum % 27;
    }

    for (idx_t i = 3; i * i <= temp; i += 2) {
        if (temp % i == 0) {
            int e = 0;
            seq_t p_mod = ((seq_t)i) % MOD;
            seq_t p7_mod = mod_pow_seq(p_mod, 7, MOD);
            while (temp % i == 0) {
                ++e;
                temp /= i;
            }
            seq_t sum = 1;
            seq_t cur = 1;
            for (int j = 0; j < e; ++j) {
                cur = (uint64_t)cur * p7_mod % 27;
                sum = (sum + cur) % 27;
            }
            sigma_mod = (uint64_t)sigma_mod * sum % 27;
        }
    }

    if (temp > 1) {          /* remaining prime factor */
        seq_t p_mod = ((seq_t)temp) % MOD;
        seq_t p7_mod = mod_pow_seq(p_mod, 7, MOD);
        seq_t sum = (1 + p7_mod) % 27;
        sigma_mod = (uint64_t)sigma_mod * sum % 27;
    }

    /* Final result */
    seq_t result = (uint64_t)n2_mod * sigma_mod % 27;
    return result;
}
