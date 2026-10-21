/*
 * A050457  a(n) = Sum_{ d divides n, d==1 mod 4} d - Sum_{ d divides n, d==3
 *          mod 4} d.
 * https://oeis.org/A050457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Sum_{d|n, d≡1 (mod 4)} d – Sum_{d|n, d≡3 (mod 4)} d
 * Range:     i8 n <= 12, i16 n <= 1216, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is multiplicative.  For a prime power p^e :
 *   if p == 2            : a(2^e) = 1
 *   if p ≡ 1 (mod 4)    : a(p^e) = (p^{e+1} – 1) / (p – 1)
 *   if p ≡ 3 (mod 4)    : a(p^e) = 1 – p + p^2 – p^3 + … + (–p)^e
 *
 * The implementation factors n with trial division and combines the
 * prime‑power contributions.  All arithmetic is done in seq_t, which
 * is at least a 64‑bit signed integer, or an arbitrary precision type
 * provided by the build system.  No dynamic memory is used.  The
 * routine works for any n ≥ 1 and is O(√n). */

#include <stdint.h>

static seq_t pow_seq(uint64_t base, uint64_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)base;
    while (exp > 0) {
        if (exp & 1ULL)
            result *= b;
        b *= b;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;          /* use 64‑bit unsigned for factoring */
    seq_t result = (seq_t)1;            /* multiplicative identity */

    /* Remove all factors of 2 – their contribution is always 1 */
    while ((x & 1ULL) == 0ULL)
        x >>= 1;

    /* Factor odd primes */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }

            if (p % 4 == 1) {          /* p ≡ 1 (mod 4) */
                seq_t pow_val = pow_seq(p, e + 1); /* p^(e+1) */
                seq_t num = pow_val - (seq_t)1;
                seq_t den = (seq_t)(p - 1);
                result *= num / den;     /* exact division */
            } else {                   /* p ≡ 3 (mod 4) */
                seq_t sum = (seq_t)0;
                seq_t term = (seq_t)1;
                for (uint64_t k = 0; k <= e; ++k) {
                    sum += term;
                    term *= -(seq_t)p;   /* multiply by –p each step */
                }
                result *= sum;
            }
        }
    }

    /* Remaining prime factor, if any */
    if (x > 1) {
        uint64_t p = x;                /* exponent e = 1 */
        if (p % 4 == 1) {              /* p ≡ 1 (mod 4) */
            seq_t pow_val = pow_seq(p, 2);   /* p^(1+1) */
            seq_t num = pow_val - (seq_t)1;
            seq_t den = (seq_t)(p - 1);
            result *= num / den;
        } else {                       /* p ≡ 3 (mod 4) */
            seq_t sum = (seq_t)1 - (seq_t)p; /* 1 + (–p) = 1 – p */
            result *= sum;
        }
    }

    return result;
}
