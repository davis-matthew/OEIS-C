/*
 * A138501  Expansion of (eta(q)^2 * eta(q^4)^4 / eta(q^2)^3)^2 in powers of
 *          q.
 * https://oeis.org/A138501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 16, i32 n <= 1684, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is multiplicative. 
 * a(1) = 1.
 * For n = 2^e : a(2^e) = -(4^e).
 * For an odd prime p:
 *   if p ≡ 1 (mod 4) : a(p^e) = ((p^2)^(e+1) – 1) / (p^2 – 1).
 *   if p ≡ 3 (mod 4) : a(p^e) = ((p^2)^(e+1) – (–1)^(e+1)) / (p^2 + 1).
 */

#include <stdint.h>

/* fast exponentiation for arbitrary‑precision seq_t */
static inline seq_t seq_pow(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) result = result * base;
        base = base * base;
        exp >>= (idx_t)1;
    }
    return result;
}

/* compute the contribution of a single prime power p^e */
static seq_t prime_power_term(uint64_t p, uint64_t e) {
    if (p == 2) {
        return -seq_pow((seq_t)4, e);
    }

    seq_t p2 = (seq_t)p * (seq_t)p;               /* p^2 */
    seq_t pow = seq_pow(p2, e + 1);               /* (p^2)^(e+1) */

    if ((p & 3) == 1) {                          /* p ≡ 1 (mod 4) */
        return (pow - (seq_t)1) / (p2 - (seq_t)1);
    } else { /* p ≡ 3 (mod 4) */
        seq_t minus1pow = ((e + 1) & 1ULL) ? (seq_t)-1 : (seq_t)1;
        return (pow - minus1pow) / (p2 + (seq_t)1);
    }
}

/* multiplicative sequence a(n) */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t tmp = (uint64_t)n;

    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p != 0) continue;

        uint64_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }

        result *= prime_power_term(p, e);
    }

    if (tmp > 1) {               /* remaining prime factor (exponent 1) */
        result *= prime_power_term(tmp, 1);
    }

    return result;
}
