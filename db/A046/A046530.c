/*
 * A046530  Number of distinct cubic residues mod n.
 * https://oeis.org/A046530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy, nice
 * Formula:   a(n) = number of distinct cubic residues mod n. * The sequence
 *            is multiplicative. For a prime power p^e (e ≥ 1) * a(p^e) = 1 +
 *            Σ_{t=0..⌊(e-1)/3⌋} φ(p^{e-3t}) / gcd(3, φ(p^{e-3t})) * where
 *            φ(p^k) = p^{k-1}(p-1). For e = 0 (n=1) the value is 1
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 91 terms through n = 964 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following function assumes that the type seq_t supports the usual
 * integer arithmetic operators.  In a C++ build seq_t may be an
 * arbitrary‐precision class; in a C build it is an unsigned integer type. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Return the number of distinct cubic residues modulo a prime power p^e. */
static seq_t prime_power_cubic_count(uint64_t p, idx_t e)
{
    seq_t part = 1;                       /* residue 0 */
    idx_t max_t = (e - 1) / 3;            /* floor((e-1)/3) */

    for (idx_t t = 0; t <= max_t; ++t) {
        idx_t k = e - 3 * t;                     /* exponent of p in modulus */
        seq_t phi = 1;                           /* φ(p^k) */
        seq_t p_pow = 1;                         /* p^{k-1} */
        for (idx_t i = 0; i < k - 1; ++i) {
            p_pow = p_pow * (seq_t)p;
        }
        phi = p_pow * (seq_t)(p - 1);            /* φ(p^k) = p^{k-1}(p-1) */

        seq_t denom = (phi % 3 == 0) ? 3 : 1;    /* gcd(3, φ) */
        seq_t units = phi / denom;               /* number of cube residues in units */
        part += units;
    }
    return part;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined but safe */
    if (n == 1) return 1;        /* a(1) = 1 */

    uint64_t primes[32];
    uint64_t exps[32];
    size_t    cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t part = prime_power_cubic_count(primes[i], exps[i]);
        result *= part;
    }
    return result;
}
