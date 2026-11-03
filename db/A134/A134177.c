/*
 * A134177  Expansion of phi(-x^3) * psi(x^4) + x * phi(-x) * psi(x^12) in
 *          powers of x where phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A134177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = b(2n+1), where b is multiplicative with b(3^e)=1,
 *            b(p^e)=e+1 for p≡1,11 (mod 24), b(p^e)=(e+1)(-1)^e for p≡5,7
 *            (mod 24), b(p^e)=(1+(-1)^e)/2 for p≡13,17,19,23 (mod 24). (For
 *            n≥0, a(0)=1.)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 9275 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t prime_factor_contribution(uint64_t p, uint64_t e) {
    if (p == 3) return (seq_t)1;
    uint64_t r = p % 24;
    if (r == 1 || r == 11) return (seq_t)(e + 1);
    if (r == 5 || r == 7) {
        seq_t val = (seq_t)(e + 1);
        if (e & 1) val = -val;
        return val;
    }
    if (r == 13 || r == 17 || r == 19 || r == 23) {
        return ((e & 1) == 0) ? (seq_t)1 : (seq_t)0;
    }
    return (seq_t)0;  /* should not occur for odd m */
}

static seq_t b_of_odd(uint64_t m) {
    if (m <= 1) return (seq_t)1;
    seq_t result = (seq_t)1;
    uint64_t n = m;
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t exp = 0;
            do { n /= p; exp++; } while (n % p == 0);
            result *= prime_factor_contribution(p, exp);
            if (result == 0) return (seq_t)0;  /* early exit */
        }
    }
    if (n > 1) {                      /* remaining prime factor */
        result *= prime_factor_contribution(n, 1);
    }
    return result;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n * 2 + 1;
    return b_of_odd(m);
}
