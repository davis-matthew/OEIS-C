/*
 * A162002  Numbers n for which 2^^n == 2^2^n (mod n); for the "^^" notation
 *          see A092188.
 * https://oeis.org/A162002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162002: a(n) = n‑th integer for which 2^^n ≡ 2^2^n (mod n) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* ---------- modular arithmetic helpers ---------- */

static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = (uint64_t)((__uint128_t)result * base % mod);
        exp >>= 1;
        if (exp)
            base = (uint64_t)((__uint128_t)base * base % mod);
    }
    return result;
}

/* ---------- Euler's totient via prime factorisation ---------- */

static uint64_t phi_u64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    uint64_t res = n;
    for (size_t i = 0; i < cnt; ++i)
        res -= res / primes[i];
    return res;
}

/* ---------- modular tetration (base 2) ---------- */

static seq_t tetr_mod(uint64_t mod, size_t k)
{
    if (mod == 1) return 0;
    if (k == 1) return (seq_t)(2 % mod);

    uint64_t phi_mod = phi_u64(mod);
    seq_t exp = tetr_mod(phi_mod, k - 1);
    uint64_t exponent = (uint64_t)exp;

    /* For even modulus 2 shares a factor with the base 2.  Adding φ(mod)
       guarantees the exponent is large enough for the non‑coprime case. */
    if ((mod & 1) == 0)
        exponent += phi_mod;

    return (seq_t)mod_pow_u64(2, exponent, mod);
}

/* ---------- compute 2^(2^n) modulo n ---------- */

static seq_t pow2_2pow_n(uint64_t mod, uint64_t n)
{
    if (mod == 1) return 0;
    uint64_t phi_mod = phi_u64(mod);
    uint64_t e_mod = mod_pow_u64(2, n, phi_mod);
    uint64_t exp = e_mod + phi_mod;   /* exponent ≥ φ(mod) for n ≥ 1 */
    return (seq_t)mod_pow_u64(2, exp, mod);
}

/* ---------- test candidate n ---------- */

static bool check_value(idx_t cand)
{
    if (cand == 0) return false;
    uint64_t mod = (uint64_t)cand;
    seq_t left  = tetr_mod(mod, (size_t)cand);
    seq_t right = pow2_2pow_n(mod, (uint64_t)cand);
    return left == right;
}

/* ---------- public sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* guard – not used */

    idx_t found = 0;
    idx_t candidate = 0;

    while (found < n) {
        ++candidate;
        if (check_value(candidate))
            ++found;
    }
    return (seq_t)candidate;
}
