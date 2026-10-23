/*
 * A176839  The number of iterations to reach 1 under the map x ->
 *          x-tau(phi(x)), starting at n.
 * https://oeis.org/A176839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A176839:
   a(n) = number of iterations to reach 1 under the map
   f(x) = x - τ(φ(x)), with τ = divisor count and φ = Euler's totient.
   a(1) = 0, a(n) for n>1 is computed iteratively.
*/

#include <stdint.h>

/* Compute Euler's totient φ(n) for 64‑bit unsigned integers. */
static uint64_t phi64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = n;
    uint64_t m = n;
    for (uint64_t p = 2; (unsigned __int128)p * p <= m; ++p) {
        if (m % p == 0) {
            result -= result / p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

/* Count the number of positive divisors τ(n) for 64‑bit unsigned integers. */
static uint64_t tau64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; (unsigned __int128)p * p <= m; ++p) {
        if (m % p == 0) {
            unsigned cnt = 0;
            while (m % p == 0) {
                ++cnt;
                m /= p;
            }
            result *= (cnt + 1);
        }
    }
    if (m > 1) result *= 2;   /* remaining prime factor */
    return result;
}

/* Main sequence function with external linkage. */
seq_t a(idx_t n)
{
    /* n is a 64‑bit unsigned integer per the build definition. */
    uint64_t val = (uint64_t)n;
    seq_t steps = 0;

    /* 1 maps directly to 0 iterations. */
    if (val <= 1) return 0;

    while (val > 1) {
        uint64_t phi = phi64(val);
        uint64_t tau = tau64(phi);
        val -= tau;
        ++steps;
    }
    return steps;
}
