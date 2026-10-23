/*
 * A046694  Ramanujan tau numbers mod 691 = sum of 11th power of divisors mod
 *          691.
 * https://oeis.org/A046694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = sum_{d|n} d^11 mod 691
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Ramanujan tau numbers modulo 691) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Auxiliary modular exponentiation.
   Works for any seq_t that supports *, % and assignment. */
static inline seq_t mod_pow_uint64(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp) {
        if (exp & 1U)
            result = (result * b) % mod;
        exp >>= 1U;
        if (exp)
            b = (b * b) % mod;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n is defined for n >= 1; for n==0 return 0 as safe fallback */
    if (n == 0)
        return 0;

    /* Special case n==1 */
    if (n == 1)
        return 1;

    const seq_t MOD = 691;
    seq_t result = 1 % MOD;
    uint64_t tmp = (uint64_t)n;

    /* Handle factor 2 separately */
    if ((tmp & 1ULL) == 0) {
        int exp = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            exp++;
        }
        seq_t p_pow11 = mod_pow_uint64(2 % MOD, 11, MOD);
        seq_t sum = 1;
        seq_t term = 1;
        for (int i = 1; i <= exp; ++i) {
            term = (term * p_pow11) % MOD;
            sum = (sum + term) % MOD;
        }
        result = (result * sum) % MOD;
    }

    /* Trial division by odd numbers up to sqrt(tmp) */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p != 0)
            continue;
        int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            exp++;
        }
        seq_t p_mod = (seq_t)(p % (uint64_t)MOD);
        seq_t p_pow11 = mod_pow_uint64(p_mod, 11, MOD);
        seq_t sum = 1;
        seq_t term = 1;
        for (int i = 1; i <= exp; ++i) {
            term = (term * p_pow11) % MOD;
            sum = (sum + term) % MOD;
        }
        result = (result * sum) % MOD;
    }

    /* If anything remains, it is a prime factor > sqrt(original n) */
    if (tmp > 1) {
        seq_t p_mod = (seq_t)(tmp % (uint64_t)MOD);
        seq_t p_pow11 = mod_pow_uint64(p_mod, 11, MOD);
        seq_t sum = (1 + p_pow11) % MOD;  /* exponent = 1 */
        result = (result * sum) % MOD;
    }

    return result;
}
