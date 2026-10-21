/*
 * A085313  Number of distinct 10th power residues modulo n.
 * https://oeis.org/A085313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of distinct 10th power residues modulo n
 * Range:     u8 n <= 513, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t gcd_uint64(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

static seq_t pow_uint(seq_t base, int exp) {
    seq_t res = 1;
    for (int i = 0; i < exp; ++i) {
        res *= base;
    }
    return res;
}

/* a(p^e) for prime p and exponent e>0 */
static seq_t prime_power_a(uint64_t p, uint8_t e) {
    if (p == 2) {
        if (e == 1 || e == 2) {
            return (seq_t)2;
        }
        /* e >= 3 */
        seq_t val = pow_uint(2, e - 3) + 1;
        return val;
    } else {
        seq_t phi = pow_uint(p, e - 1) * (seq_t)(p - 1);
        uint64_t phi_mod10 = (uint64_t)(phi % (seq_t)10);
        uint64_t g = gcd_uint64(10, phi_mod10);
        seq_t img = phi / (seq_t)g;
        return img + 1;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;       /* not used, safety */
    uint64_t tmp = (uint64_t)n;
    seq_t result = 1;
    /* trial division up to sqrt(n) */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint8_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            result *= prime_power_a(p, e);
        }
    }
    if (tmp > 1) {
        /* remaining prime factor with exponent 1 */
        result *= prime_power_a(tmp, 1);
    }
    return result;
}
