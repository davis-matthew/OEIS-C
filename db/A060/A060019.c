/*
 * A060019  a(n) = floor(2*sqrt(prime(n)-2)) where prime(n) = n-th prime.
 * https://oeis.org/A060019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2*sqrt(prime(n)-2))
 * Range:     u8 n <= 18, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  For n < 1 the result is irrelevant. */
    if (n == 0)
        return 0;

    /* Obtain the n-th prime.  get_prime(1) == 2. */
    uint32_t p = get_prime(n);

    /* x = prime(n) - 2 */
    seq_t x = (seq_t)p - 2;

    /* Integer square root of x. */
    seq_t s = floor_sqrt(x);

    /* Use 64‑bit arithmetic to avoid overflow of s*s. */
    unsigned long long s_ull = (unsigned long long)s;
    unsigned long long x_ull = (unsigned long long)x;

    /* Threshold: x >= s^2 + s + 1  ⇔  floor(2*sqrt(x)) == 2*s + 1. */
    unsigned long long threshold = s_ull * s_ull + s_ull + 1ULL;

    unsigned long long res = 2ULL * s_ull;
    if (x_ull >= threshold)
        ++res;

    return (seq_t)res;
}
