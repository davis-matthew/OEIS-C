/*
 * A087412  a(n) is the number of solutions to x^3 + y^3 == 1 (mod n).
 * https://oeis.org/A087412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) is multiplicative with the following prime‐power formulas:
 *            • a(2^e) = 2^e (p = 2, p ≡ 2 (mod 3)) • a(3^e) = 3 e = 1 •
 *            a(3^e) = 2·3^e e > 1 • a(p^e) = p^e p ≡ 2 (mod 3) • a(p^e) =
 *            p^(e‑1)·(p‑1 + Σ Legendre(12·b⁻¹ – 3·b² , p)) for p ≡ 1 (mod 3),
 *            p ≥ 5 a(n) for general n is the product of its p...
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, mod_pow, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/mod_pow.h"

/* Legendre symbol (value/p) for an odd prime p. Returns 1, 0, or –1. */
static int legendre_symbol(uint64_t value, uint64_t prime)
{
    uint64_t v = value % prime;
    if (v == 0) return 0;
    seq_t pow_res = mod_pow((seq_t)v, (prime - 1) / 2, (seq_t)prime);
    if (pow_res == (seq_t)(prime - 1)) return -1;
    return 1;
}

/* Main shortcut helper: return b⁻¹ mod p.  Usable only for prime p. */
static uint64_t mod_inv_prime(uint64_t b, uint64_t p)
{
    /* Fermat's little theorem: b^(p‑2) ≡ b⁻¹ (mod p) */
    seq_t inv = mod_pow((seq_t)b, p - 2, (seq_t)p);
    return (uint64_t)inv;
}

/* ---------------------------------------------------------- */
/*  Main sequence function                                       */
/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined; return 0 for safety */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t block = 1;

        if (p == 3) {
            if (e == 1)
                block = (seq_t)3;
            else
                block = 2 * pow_uint(3, e);
        }
        else if (p % 3 == 2) {
            block = pow_uint(p, e);
        }
        else { /* p % 3 == 1  (p ≥ 5) */
            seq_t p_pow = pow_uint(p, e - 1);

            int64_t sum = 0;
            for (uint64_t b = 1; b < p; ++b) {
                uint64_t inv_b = mod_inv_prime(b, p);
                uint64_t term1 = (12 * inv_b) % p;
                uint64_t term2 = (3 * ((b * b) % p)) % p;
                uint64_t val   = (term1 + p - term2) % p;
                int leg = legendre_symbol(val, p);
                sum += leg;              /* sum fits in 64‑bit for these p */
            }

            seq_t part = (seq_t)(p - 1 + sum);   /* guaranteed ≥ 0 */
            block = p_pow * part;
        }

        result *= block;
    }

    return result;
}
