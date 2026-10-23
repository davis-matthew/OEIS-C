/*
 * A138512  Expansion of q * f(q^5)^5 / f(q) in powers of q where f() is a
 *          Ramanujan theta function.
 * https://oeis.org/A138512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) is multiplicative. a(1) = 1. For prime powers p^e (e >= 1):
 *            a(2^e) = -(2^(e+1) - (-1)^(e+1)) / 3 a(5^e) = 5^e if p ≡ 1 or 4
 *            (mod 5): a(p^e) = (p^(e+1) - 1) / (p - 1) if p ≡ 2 or 3 (mod 5):
 *            a(p^e) = (p^(e+1) + (-1)^e) / (p + 1)
 * Range:     i8 n <= 12, i16 n <= 250, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* power in 2‑squaring style, works for any seq_t type */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* contribution of a single prime power p^e */
static seq_t contrib_prime(uint64_t p, uint64_t e)
{
    if (p == 2) {
        seq_t powv   = pow_seq((seq_t)2, e + 1);
        seq_t alt    = ((e + 1) & 1ULL) ? (seq_t)-1 : (seq_t)1;   /* (-1)^(e+1) */
        seq_t num    = powv - alt;
        return - (num / (seq_t)3);
    }
    if (p == 5) {
        return pow_seq((seq_t)5, e);
    }
    seq_t powv  = pow_seq((seq_t)p, e + 1);
    uint64_t mod5 = p % 5;
    if (mod5 == 1 || mod5 == 4) {
        seq_t num = powv - (seq_t)1;
        seq_t den = (seq_t)(p - 1);
        return num / den;
    }
    /* mod5 == 2 or 3 */
    seq_t alt = (e & 1ULL) ? (seq_t)-1 : (seq_t)1;      /* (-1)^e */
    seq_t num = powv + alt;
    seq_t den = (seq_t)(p + 1);
    return num / den;
}

/* Main sequence entry function */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;       /* a(1) = 1, a(0) unused */

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            result *= contrib_prime(p, e);
        }
    }
    if (m > 1) {                     /* remaining prime factor */
        result *= contrib_prime(m, 1);
    }
    return result;
}
