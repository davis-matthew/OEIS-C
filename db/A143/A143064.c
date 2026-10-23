/*
 * A143064  Expansion of a Ramanujan false theta series variation of A089801
 *          in powers of x.
 * https://oeis.org/A143064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(3*n + 1), where b(m) is multiplicative: - if m contains
 *            a factor 3, b(m) = 0 - if m is not a perfect square, b(m) = 0 -
 *            otherwise let m = s² with s ≥ 1 and s not divisible by 3. For
 *            prime factorisations of s = ∏ p^e : * for p = 2: toggle sign if
 *            e is even * for p ≡ 5 (mod 6): toggle sign...
 * Range:     i8 n <= 55, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root floor: 0 ≤ r ≤ sqrt(n) */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n >> 1;
    if (hi < 2) hi = 2;          /* at least 2 for the loop to run */
    hi++;

    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0 || n / mid >= mid) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* m = 3*n + 1 */
    seq_t m = (seq_t)n * 3 + 1;

    /* Convert to uint64_t for the small‑n regime we test.
       The sequence values grow very slowly, so m fits in 64 bits
       for the expected test ranges. */
    uint64_t um = (uint64_t)m;

    /* If 3 divides m, result is 0 */
    if (um % 3ULL == 0ULL) {
        return (seq_t)0;
    }

    /* Check if m is a perfect square */
    uint64_t s = isqrt_u64(um);
    if (s * s != um) {
        return (seq_t)0;
    }

    /* s must not contain the prime 3 */
    if (s % 3ULL == 0ULL) {
        return (seq_t)0;
    }

    /* Compute the sign from the factorisation of s */
    int sign = 1;
    uint64_t tmp = s;
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            unsigned exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (p == 2ULL) {
                /* toggle if exponent is even */
                if ((exp & 1ULL) == 0ULL) {
                    sign = -sign;
                }
            } else {
                /* p ≡ 5 (mod 6) → toggle if exponent is odd */
                if (p % 6ULL == 5ULL) {
                    if ((exp & 1ULL) == 1ULL) {
                        sign = -sign;
                    }
                }
                /* p ≡ 1 (mod 6) → no effect */
            }
        }
    }

    /* If a prime factor remains (tmp > 1) */
    if (tmp > 1ULL) {
        /* tmp is prime; exponent is 1 */
        if (tmp == 2ULL) {
            /* exponent is 1 → odd, so no toggle */
        } else if (tmp % 6ULL == 5ULL) {
            /* toggle because exponent 1 is odd */
            sign = -sign;
        }
    }

    return (seq_t)sign;
}
