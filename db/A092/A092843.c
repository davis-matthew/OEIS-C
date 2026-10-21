/*
 * A092843  a(n) = Sum_{k|n, k>1} phi(k-1), where phi() is the Euler phi
 *          function.
 * https://oeis.org/A092843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k|n, k>1} phi(k-1)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a straightforward divisor enumeration and an inline totient routine. */
/* The function works for all n >= 1, with a(1) = 0. */

#include <stdint.h>

/* Euler's totient function for a 64‑bit integer n (n >= 1). */
static inline seq_t phi(idx_t n)
{
    if (n == 0) return 0;      /* unused in this sequence but safe */
    seq_t result = n;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition, no divisors >1. */
    if (n <= 1) return 0;

    /* Start with the divisor k = n itself. */
    seq_t sum = phi(n - 1);

    /* Enumerate proper divisors k > 1 of n. */
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += phi(d - 1);
            idx_t other = n / d;
            if (other != d)            /* add the paired divisor once */
                sum += phi(other - 1);
        }
    }

    return sum;
}
