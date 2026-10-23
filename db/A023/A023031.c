/*
 * A023031  Number of compositions of n into 6 ordered relatively prime parts.
 * https://oeis.org/A023031
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * C((n/d)-1,5)
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 224, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   _mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_mobius.h"

/* compute gcd of two unsigned integers */
static inline uint64_t _gcd_u64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* compute binomial coefficient C(n,5) for n >= 5 */
static seq_t _binom5(uint64_t n) {
    if (n < 5) return 0;
    uint64_t denom = 120;                  /* 5! = 120 */
    seq_t result = 1;
    uint64_t f[5] = {n, n-1, n-2, n-3, n-4};

    for (int i = 0; i < 5; ++i) {
        uint64_t val = f[i];
        uint64_t g = _gcd_u64(val, denom);
        if (g > 1) {
            val   /= g;
            denom /= g;
        }
        result *= (seq_t)val;
    }
    if (denom > 1) result /= (seq_t)denom;   /* safety for any leftover divisor */
    return result;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n < 6) return 0;          /* undefined below offset, return 0 */
    signed long long total = 0;

    for (uint64_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;

        /* process divisor d */
        int mu = _mobius(d);
        if (mu != 0) {
            uint64_t t = n / d - 1;
            seq_t bin = _binom5(t);
            if (mu > 0) total += (signed long long)bin;
            else       total -= (signed long long)bin;
        }

        /* symmetrically process n/d if distinct */
        uint64_t d2 = n / d;
        if (d2 != d) {
            mu = _mobius(d2);
            if (mu != 0) {
                uint64_t t = n / d2 - 1;
                seq_t bin = _binom5(t);
                if (mu > 0) total += (signed long long)bin;
                else       total -= (signed long long)bin;
            }
        }
    }
    return (seq_t)total;
}
