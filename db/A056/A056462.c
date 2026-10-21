/*
 * A056462  Number of primitive (aperiodic) palindromes using a maximum of six
 *          different symbols.
 * https://oeis.org/A056462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * 6^{ceil((n/d)/2)}
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where mu(d) is the Möbius function. */

#include <stdint.h>
#include "../utils/pow6.h"

/* Compute the Möbius function μ(n).  n >= 1. */
static int mobius(idx_t n)
{
    if (n == 1)
        return 1;
    int primes = 0;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) {
                n /= p;
                ++cnt;
                if (cnt > 1)    /* square factor → μ = 0 */
                    return 0;
            }
            ++primes;
        }
    }
    if (n > 1)          /* remaining prime factor */
        ++primes;
    return (primes & 1) ? -1 : 1;
}

/* Primary sequence function.  n >= 1. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* divisor d */
        int mu = mobius(d);
        if (mu) {
            idx_t len = (n / d + 1) / 2;    /* ceil((n/d)/2) */
            seq_t val = pow6(len);
            total += (mu > 0) ? val : -val;
        }

        /* paired divisor n/d, skip if equal to d */
        idx_t d2 = n / d;
        if (d2 == d)
            continue;

        mu = mobius(d2);
        if (mu) {
            idx_t len = (n / d2 + 1) / 2;
            seq_t val = pow6(len);
            total += (mu > 0) ? val : -val;
        }
    }
    return total;
}
