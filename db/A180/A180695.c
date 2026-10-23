/*
 * A180695  Smallest power of 14 that begins with n.
 * https://oeis.org/A180695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   digits, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the smallest power of 14 that begins with the integer n. */
/* The algorithm uses the leading‑digit test based on fractional parts of e*log10(14). */

#include <math.h>
#include <stdbool.h>
#include "../utils/digits.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Special case: n == 1 equals 14^0. */
    if (n == 1)
        return pow_uint(14, 0);

    /* Number of decimal digits of n. */
    int len = digits(n);
    if (len <= 0)
        return 0;      /* should not happen for n >= 1 */

    /* Base10 logarithm of 14, constant in long double. */
    const long double LOG10_14 = 1.146128035035088373774051845L;

    /* Find the minimal exponent e such that 14^e has at least len digits. */
    idx_t e = 0;
    while ((long double)e * LOG10_14 < (long double)(len - 1) - 1e-12L)
        ++e;

    /* Iterate until the leading len digits equal n. */
    for (;; ++e) {
        /* Fractional part of e*log10(14). */
        long double frac = (long double)e * LOG10_14;
        frac -= floorl(frac);

        /* Compute the first len decimal digits of 14^e. */
        long double tmp = powl(10.0L, frac + len - 1.0L);
        unsigned long long front = (unsigned long long)(floorl(tmp + 1e-12L));

        if ((unsigned long long)n == front)
            /* 14^e fits in seq_t for arbitrary precision builds; pow_uint
             * constructs the value using arbitrary-precision arithmetic.
             */
            return pow_uint(14, e);
    }
}
