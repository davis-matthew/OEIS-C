/*
 * A066975  a(n) = gcd(binomial(2n,n), 2^n + 1).
 * https://oeis.org/A066975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( binomial(2n,n), 2^n + 1 )
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 781, u64 n <= 1169,
 *            u128 n <= 9281, big any n
 * Helpers:   ensure_primes_upto, v_p_in_central_binom, v_p_in_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_primes_upto.h"
#include "../utils/v_p_in_central_binom.h"
#include "../utils/v_p_in_factorial.h"

/* Utility: modular exponentiation  (handles mod up to 64‑bit) */
static inline uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp > 0)
    {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* Utility: small integer power (base up to 32‑bit, exponent up to 32‑bit) */
static inline uint64_t pow_uint64(uint64_t base, uint32_t exp)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* exponent of prime p in 2^n + 1 (p odd). */
static inline idx_t v_p_in_two_pow_plus_one(idx_t n, idx_t p)
{
    /* 2^n + 1 is odd, so p != 2 */
    idx_t exp = 0;
    uint64_t mod = (uint64_t)p;
    uint64_t target = mod - 1;   /* 2^n ≡ -1 (mod p) */

    while (true)
    {
        uint64_t val = pow_mod_uint64(2ULL, n, mod);
        if (val != target) break;          /* stop when not divisible */

        ++exp;

        /* prepare for next higher power of p, guard against overflow */
        if (mod > (UINT64_MAX / (uint64_t)p))
            break;                         /* too large, cannot happen for p ≤ 2n */
        mod *= (uint64_t)p;
        target = mod - 1;
    }
    return exp;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* 2^n + 1 is always odd, so gcd will never contain factor 2 */
    /* central binomial coefficient exponent computation */
    idx_t limit = 2 * n;
    ensure_primes_upto(limit);

    /* local static storage for primes array - obtain pointer from ensure_primes_upto */
    /* Since ensure_primes_upto only keeps data in static context, we need its helper.
       We'll recreate the primes array here by reimplementing a light version: */
    /* For simplicity and due to build constraints, we replicate a minimal prime list
       generation limited to <= limit. */
    /* WARNING: This block must be consistent with ensure_primes_upto implementation. */

    /* Re-generate primes (small list, limit <= 20000 for n up to 10000) */
    size_t sieve_sz = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(sieve_sz);
    for (size_t i = 0; i < sieve_sz; ++i) is_prime[i] = true;
    if (sieve_sz > 0) { is_prime[0] = false; if (sieve_sz > 1) is_prime[1] = false; }
    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    seq_t result = 1;

    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
        {
            idx_t p = (idx_t)i;
            idx_t e_bin = v_p_in_central_binom(n, p);
            if (e_bin == 0) continue;

            idx_t e_pow = v_p_in_two_pow_plus_one(n, p);
            idx_t e = e_bin < e_pow ? e_bin : e_pow;
            if (e == 0) continue;

            /* multiply result by p^e (safe because e small and result fits in seq_t) */
            for (idx_t k = 0; k < e; ++k)
                result *= (seq_t)p;
        }

    free(is_prime);
    return result;
}
