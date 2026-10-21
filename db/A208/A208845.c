/*
 * A208845  Expansion of f(x)^2 in powers of x where f() is a Ramanujan theta
 *          function.
 * https://oeis.org/A208845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * b(12*n + 1) where b is multiplicative. b(2^e) =
 *            b(3^e) = 0^e (i.e. b is zero if n has factor 2 or 3). For a
 *            prime power p^e: if p ≡ 7 or 11 (mod 12) : b(p^e) = (1+(-1)^e)/2
 *            (zero for odd e). if p ≡ 5 (mod 12) : b(p^e) = (-1)^(e/2) *
 *            (1+(-1)^e)/2 (zero for odd e). if p ≡ 1 (mod 12)...
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for idx_t (non‑negative). */
static inline idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi)
    {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Multiplicative factor b(m).  Returns 0 if b(m)=0. */
static seq_t b_value(idx_t m)
{
    if ((m & 1) == 0 || (m % 3) == 0)   /* contains 2 or 3 */
        return 0;

    seq_t result = 1;
    idx_t temp = m;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p != 0)
            continue;

        idx_t e = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++e;
        }

        int mod12 = (int)(p % 12);
        if (mod12 == 7 || mod12 == 11)
        {
            if (e & 1)           /* odd exponent -> zero */
                return 0;
            /* even exponent -> factor 1, nothing to multiply */
        }
        else if (mod12 == 5)
        {
            if (e & 1)
                return 0;
            int half = (int)(e >> 1);
            int sign = (half & 1) ? -1 : 1;
            result *= (seq_t)sign;
        }
        else if (mod12 == 1)
        {
            /* Find representation p = x² + 9y² with y ≥ 1. */
            bool found = false;
            int y = 0;
            for (idx_t yy = 1; 9 * yy * yy <= p; ++yy)
            {
                idx_t t = p - 9 * yy * yy;
                idx_t r = isqrt_idx(t);
                if (r * r == t)
                {
                    y = (int)yy;
                    found = true;
                    break;
                }
            }
            if (!found)
                return 0;           /* no representation -> zero */

            int sign = 1;
            if (e & 1)             /* odd exponent */
                sign = (y & 1) ? -1 : 1;   /* (-1)^y */

            result *= (seq_t)(e + 1);
            result *= (seq_t)sign;
        }
        else
        {
            return 0;               /* should not happen for prime >3 */
        }
    }

    if (temp > 1)            /* remaining prime factor */
    {
        idx_t p = temp;
        idx_t e = 1;        /* exponent is 1 */

        int mod12 = (int)(p % 12);
        if (mod12 == 7 || mod12 == 11)
        {
            if (e & 1)
                return 0;
        }
        else if (mod12 == 5)
        {
            if (e & 1)
                return 0;
            int half = (int)(e >> 1);
            int sign = (half & 1) ? -1 : 1;
            result *= (seq_t)sign;
        }
        else if (mod12 == 1)
        {
            bool found = false;
            int y = 0;
            for (idx_t yy = 1; 9 * yy * yy <= p; ++yy)
            {
                idx_t t = p - 9 * yy * yy;
                idx_t r = isqrt_idx(t);
                if (r * r == t)
                {
                    y = (int)yy;
                    found = true;
                    break;
                }
            }
            if (!found)
                return 0;   /* no representation -> zero */

            int sign = 1;
            if (e & 1)
                sign = (y & 1) ? -1 : 1;

            result *= (seq_t)(e + 1);
            result *= (seq_t)sign;
        }
        else
        {
            return 0;
        }
    }

    return result;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    idx_t m = 12 * n + 1;
    seq_t val = b_value(m);
    if (n & 1)            /* multiply by (-1)^n */
        val = -val;
    return val;
}
