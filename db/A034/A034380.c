/*
 * A034380  Ratio of totient to Carmichael's lambda function: a(n) =
 *          A000010(n) / A002322(n).
 * https://oeis.org/A034380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(n) / λ(n) (Euler's totient over Carmichael's function)
 * Range:     u8 n <= 1795, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static inline seq_t seq_pow_uint(seq_t base, idx_t exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

static inline seq_t seq_gcd(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1; undefined for 0 */

    /* φ(n) computation */
    seq_t phi = (seq_t)n;
    /* factorization arrays (max 64 distinct primes for 64‑bit numbers) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;
    factor_u64(n, primes, exps, &count);
    for (size_t i = 0; i < count; ++i) {
        seq_t p = (seq_t)primes[i];
        phi -= phi / p;
    }

    /* λ(n) computation */
    seq_t lam = 1;
    for (size_t i = 0; i < count; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];
        seq_t part;
        if (p == 2) {
            if (e == 1) {
                part = 1;
            } else if (e == 2) {
                part = 2;
            } else {
                part = seq_pow_uint(2, e - 2);
            }
        } else {
            seq_t p_pow = seq_pow_uint(p, e - 1);
            part = p_pow * (p - 1);
        }
        seq_t g = seq_gcd(lam, part);
        lam = lam / g * part;
    }

    return phi / lam;
}
