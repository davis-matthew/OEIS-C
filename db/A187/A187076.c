/*
 * A187076  Coefficients of L-series for elliptic curve "144a1": y^2 = x^3 -
 *          1.
 * https://oeis.org/A187076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 325, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A187076
    a(n) = b(6*n + 1)
    b is multiplicative:
        b(p^e) = b(p)*b(p^(e-1)) - p*b(p^(e-2))     for e ≥ 2
    base values:
        If p ≡ 5 (mod 6)      → b(p) = 0
        If p ≡ 1 (mod 6) then
            write p = x² + 3*y²  (x>0, y≥0)
            sign = +1 if x ≡ 1 or 2 (mod 6), else –1
            b(p) = sign * 2*x
    (b(1) = 1)
*/

#include <stdint.h>
#include <stddef.h>

/* integer square root (floor) for unsigned 64‑bit values */
static inline uint64_t isqrt_uint64(uint64_t n)
{
    if (n <= 1U) return n;
    uint64_t lo = 0, hi = n, mid, res = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        /* mid is never 0 here because n > 1, lo ≥ 0, hi ≥ 1 */
        if (mid <= n / mid) {            /* mid*mid ≤ n */
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

/* base value b(p) for prime p (p % 6 == 1 or 5) */
static inline seq_t prime_base(uint64_t p)
{
    if ((p % 6) == 5) {
        return (seq_t)0;
    }
    /* p ≡ 1 (mod 6) – find representation p = x² + 3*y² */
    uint64_t x = 0;
    for (uint64_t y = 0; ; ++y) {
        uint64_t term = 3ULL * y * y;
        if (term > p) break;
        uint64_t val = p - term;
        uint64_t xx = isqrt_uint64(val);
        if (xx * xx == val) {
            x = xx;
            break;
        }
    }
    int sgn = (x % 6 == 1 || x % 6 == 2) ? 1 : -1;
    return sgn * (seq_t)(2U * x);
}

/* compute b(m) for any positive integer m */
static seq_t compute_b(uint64_t m)
{
    seq_t result = 1;
    uint64_t temp = m;

    /* trial division for small primes */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            seq_t base = prime_base(p);

            /* compute b(p^e) using the recurrence */
            seq_t f0 = 1;          /* b(p^0) */
            seq_t f1 = base;       /* b(p^1) */
            seq_t f2;
            for (unsigned k = 2; k <= e; ++k) {
                f2 = base * f1 - (seq_t)p * f0;
                f0 = f1;
                f1 = f2;
            }
            seq_t coeff = f1;
            result *= coeff;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        seq_t base = prime_base(temp);
        result *= base;
    }

    return result;
}

seq_t a(idx_t n)
{
    /* a(0) = 1, generally a(n) = b(6*n + 1) for n ≥ 0 */
    uint64_t m = 6ULL * n + 1ULL;
    return compute_b(m);
}
