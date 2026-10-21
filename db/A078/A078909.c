/*
 * A078909  Let r+i*s be the sum, with multiplicity, of the first-quadrant
 *          Gaussian primes dividing n; sequence gives s values.
 * https://oeis.org/A078909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p^e∥n} e * coeff(p) coeff(2)=2; coeff(p)=0 if p≡3 (mod
 *            4); coeff(p)=a+b for p≡1 (mod 4) where p=a²+b²
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static inline void rep(uint64_t p, uint64_t *a, uint64_t *b) {
    uint64_t limit = (uint64_t)floor_sqrt((seq_t)p);
    for (uint64_t x = 1; x <= limit; ++x) {
        seq_t rest = (seq_t)p - (seq_t)x * (seq_t)x;
        uint64_t y = (uint64_t)floor_sqrt(rest);
        if ((seq_t)y * (seq_t)y == rest) {
            *a = x;
            *b = y;
            return;
        }
    }
    *a = 0; *b = 0;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    seq_t sum = 0;
    /* account for factor 2 */
    while ((n & 1ULL) == 0) {
        sum += 2;
        n >>= 1;
    }
    /* trial division for odd primes */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if ((p & 3ULL) == 1) {          /* p ≡ 1 (mod 4) */
                uint64_t a_part, b_part;
                rep(p, &a_part, &b_part);
                sum += (seq_t)exp * (seq_t)(a_part + b_part);
            }
            /* p ≡ 3 (mod 4) contributes 0 */
        }
    }
    /* remaining prime factor, if any */
    if (n > 1) {
        if ((n & 3ULL) == 1) {
            uint64_t a_part, b_part;
            rep(n, &a_part, &b_part);
            sum += (seq_t)(a_part + b_part);
        }
    }
    return sum;
}
