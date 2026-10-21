/*
 * A050980  Haupt-exponents of 8 modulo integers relatively prime to 8.
 * https://oeis.org/A050980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of 8 modulo (2n+1)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   factor_u64, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The index n ≥ 1.  The odd modulus m = 2n+1.  The function returns the
    smallest positive integer e such that 8^e ≡ 1 (mod m).
    This is the standard multiplicative order of 8 in the reduced
    residue class ring modulo m.  It is computed by:
         1.  φ(m) is obtained by prime‑factorising m.
         2.  All distinct prime factors of φ(m) are collected.
         3.  Starting from order = φ(m), each prime factor p is tried
             in turn: while order % p == 0 and 8^(order/p) ≡ 1 (mod m)
             the exponent order is divided by p.
    All integer arithmetic is performed in 64‑bit unsigned type; the final
    result is cast to seq_t.  This implementation relies on the helper
    functions 'factor_u64' and 'powmod64' supplied in the build. */

#include <stdint.h>
#include <stddef.h>

#include "../utils/factor_u64.h"
#include "../utils/powmod64.h"

seq_t a(idx_t n)
{
    /* Modulus m = 2n+1 (odd, ≥ 3 for n ≥ 1). */
    uint64_t m = (uint64_t)n * 2ull + 1ull;

    /* ---------- 1. φ(m)  ---------- */
    uint64_t primes_m[64], exps_m[64];
    size_t cnt_m;
    factor_u64(m, primes_m, exps_m, &cnt_m);

    uint64_t phi = 1ull;
    for (size_t i = 0; i < cnt_m; ++i) {
        uint64_t p = primes_m[i];
        uint64_t e = exps_m[i];
        /* p^(e-1) */
        uint64_t pow_pe1 = 1ull;
        for (uint64_t k = 0; k < e - 1; ++k)
            pow_pe1 *= p;
        /* update φ */
        phi = (uint64_t)((__int128)phi * pow_pe1 * (p - 1ull));
    }

    /* ---------- 2. factor φ(m) ---------- */
    uint64_t primes_phi[64], exps_phi[64];
    size_t cnt_phi;
    factor_u64(phi, primes_phi, exps_phi, &cnt_phi);

    /* ---------- 3. compute multiplicative order ---------- */
    uint64_t order = phi;
    for (size_t i = 0; i < cnt_phi; ++i) {
        uint64_t p = primes_phi[i];
        /* consider each distinct prime factor */
        while (order % p == 0ull) {
            uint64_t cand = order / p;
            if (powmod64(8ull, cand, m) == 1ull)
                order = cand;
            else
                break;
        }
    }

    return (seq_t)order;
}
