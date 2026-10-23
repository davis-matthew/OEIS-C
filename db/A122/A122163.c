/*
 * A122163  Expansion of f(-q)^2*P(q) in powers of q.
 * https://oeis.org/A122163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(12*n+1) b is multiplicative. b(2^e) = b(3^e) = 0^e
 *            b(p^e) = (1+(-1)^e)/2 * p^e if p ≡ 7,11 (mod 12) b(p^e) =
 *            (-1)^(e/2) * (1+(-1)^e)/2 * p^e if p ≡ 5 (mod 12) b(p^e) =
 *            (e+1)*((-1)^y*p)^e if p ≡ 1 (mod 12) and p = x²+9y² (the
 *            definitions are taken from the OEIS entry A122163)
 * Range:     i8 n <= 5, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   rep_x_y
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/rep_x_y.h"

static seq_t seq_pow_uint64(uint64_t base, int exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    for (int i = 0; i < exp; ++i)
        result *= b;
    return result;
}

/* Compute the multiplicative factor for a prime power p^e. */
static bool factor_pm_one(uint64_t p, int e, seq_t *factor)
{
    seq_t f = 0;
    if (p == 2 || p == 3) {
        f = 0;
    } else if (p % 12 == 7 || p % 12 == 11) {
        f = (e & 1) ? 0 : seq_pow_uint64(p, e);
    } else if (p % 12 == 5) {
        if (e & 1) {
            f = 0;
        } else {
            int sign = ((e >> 1) & 1) ? -1 : 1;
            f = (seq_t)sign * seq_pow_uint64(p, e);
        }
    } else if (p % 12 == 1) {
        uint64_t y;
        if (!rep_x_y(p, &y))
            f = 0;
        else {
            seq_t p_pow = seq_pow_uint64(p, e);
            int sign = (y & 1) ? -1 : 1;
            int sign_pow = (e & 1) ? sign : 1;
            f = (seq_t)(e + 1) * (seq_t)sign_pow * p_pow;
        }
    } else {
        f = 0;
    }
    *factor = f;
    return f != 0;
}

seq_t a(idx_t n)
{
    uint64_t m = 12ULL * n + 1ULL; /* m = 12*n + 1 */
    uint64_t tmp = m;
    seq_t result = 1;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                ++e;
                tmp /= p;
            }
            seq_t factor;
            if (!factor_pm_one(p, e, &factor))
                return 0;
            result *= factor;
        }
    }

    if (tmp > 1) { /* remaining prime factor */
        seq_t factor;
        if (!factor_pm_one(tmp, 1, &factor))
            return 0;
        result *= factor;
    }

    return result;
}
