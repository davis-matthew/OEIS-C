/*
 * A045763  Number of numbers "unrelated to n": m < n such that m is neither a
 *          divisor of n nor relatively prime to n.
 * https://oeis.org/A045763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n + 1 - d(n) - φ(n) (offset = 1)
 * Range:     u8 n <= 388, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the divisor count d(n) for n <= UINT64_MAX
 * Result is returned in type seq_t. */
static seq_t divisor_count_seq(idx_t n)
{
    idx_t m = n;
    seq_t d = 1;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            d *= (seq_t)(exp + 1);
        }
    }
    if (m > 1) {            /* remaining prime factor */
        d *= (seq_t)2;
    }
    return d;
}

/* Compute Euler's totient φ(n) for n <= UINT64_MAX
 * Result is returned in type seq_t. */
static seq_t phi_seq(idx_t n)
{
    idx_t m = n;
    seq_t phi = (seq_t)n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            while (m % p == 0) {
                m /= p;
            }
            phi = phi / p * (p - 1);   /* exact arithmetic */
        }
    }
    if (m > 1) {            /* remaining prime factor */
        phi = phi / m * (m - 1);
    }
    return phi;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for offset 1 */
        return 0;

    seq_t n_seq = (seq_t)n;
    seq_t d     = divisor_count_seq(n);
    seq_t phi   = phi_seq(n);
    return n_seq + 1 - d - phi;
}
