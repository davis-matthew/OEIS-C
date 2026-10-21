/*
 * A055677  Smallest k > n where n is a 6th power modulo k.
 * https://oeis.org/A055677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k > n such that n is a 6th power residue modulo
 *            k. Implements a brute‑force search with efficient residue checks
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"

/* Euclidean gcd for 64‑bit integers. */

/* Modular exponentiation for arbitrary precision seq_t. */
static seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) {
    seq_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* Check whether n is a 6th power residue modulo k. */
static bool is_6th_power_residue_modulo_k(uint64_t n, uint64_t k) {
    if (n == 0) return true;      /* 0 is a 6th power mod every k   */
    uint64_t tmp_k = k;

    /* Handle factor 2 first. */
    if (tmp_k % 2 == 0) {
        uint64_t e = 0;               /* exponent of 2 in k          */
        while (tmp_k % 2 == 0) {
            tmp_k /= 2;
            ++e;
        }

        /* valuation of n at 2 */
        uint64_t s = 0;
        uint64_t tmp = n;
        while (s < e && (tmp & 1ULL) == 0) {
            tmp >>= 1;
            ++s;
        }
        if (s < e) {
            if (s % 6 != 0) return false;
            uint64_t exp_mod = e - s;     /* how many 2‑factors remain */

            if (exp_mod >= 3) {
                if ((tmp & 7ULL) != 1) return false;   /* modulo 8 */
            } else if (exp_mod == 2) {
                if ((tmp & 3ULL) != 1) return false;   /* modulo 4 */
            } else /* exp_mod == 1 */ {
                /* tmp must be odd; always true for the unit part   */
            }
        }
        /* else s >= e → residue 0, always a 6th power */
    }

    /* Handle odd prime factors. */
    for (uint64_t p = 3; p <= tmp_k / p; p += 2) {
        if (tmp_k % p == 0) {
            uint64_t e = 0;
            while (tmp_k % p == 0) {
                tmp_k /= p;
                ++e;
            }

            /* valuation of n at p */
            uint64_t s = 0;
            uint64_t tmp = n;
            while (s < e && tmp % p == 0) {
                tmp /= p;
                ++s;
            }

            if (s < e) {
                if (s % 6 != 0) return false;

                uint64_t exp_mod = e - s;          /* exponent for unit part */

                /* modulus = p^exp_mod  (exp_mod ≥ 1) */
                seq_t modulus = 1;
                for (uint64_t i = 0; i < exp_mod; ++i) modulus *= p;

                /* φ(p^exp_mod) = p^(exp_mod-1)·(p−1) */
                seq_t pe1 = 1;
                for (uint64_t i = 0; i < exp_mod - 1; ++i) pe1 *= p;
                seq_t phi = pe1 * (p - 1);

                uint64_t g = gcd_uint64(6, (uint64_t)phi);
                uint64_t exp = (uint64_t)(phi) / g;      /* φ/g */

                seq_t u_mod = tmp % modulus;
                if (mod_pow(u_mod, exp, modulus) != (seq_t)1) return false;
            }
        }
    }

    /* Remaining prime factor (if any). */
    if (tmp_k > 1) {
        uint64_t p = tmp_k;   /* exponent e = 1 */
        uint64_t s = 0;
        uint64_t tmp = n;
        while (s < 1 && tmp % p == 0) {
            tmp /= p;
            ++s;
        }
        if (s < 1) {          /* n not divisible by p at all */
            if (s % 6 != 0) return false;   /* always true but keep for consistency */

            seq_t modulus = p;                 /* p^1 */
            seq_t phi = p - 1;
            uint64_t g = gcd_uint64(6, (uint64_t)phi);
            uint64_t exp = (uint64_t)(phi) / g;
            seq_t u_mod = tmp % modulus;
            if (mod_pow(u_mod, exp, modulus) != (seq_t)1) return false;
        }
        /* else s >= 1 → residue 0, always a 6th power            */
    }

    return true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* a(0) = 1 by definition      */
    uint64_t nval = (uint64_t)n;
    for (uint64_t k = nval + 1; ; ++k) {
        if (is_6th_power_residue_modulo_k(nval, k))
            return (seq_t)k;
    }
}
