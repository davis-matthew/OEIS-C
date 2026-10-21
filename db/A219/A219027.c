/*
 * A219027  Number of non-primitive roots for n, less than n.
 * https://oeis.org/A219027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-1 - (#primitive roots modulo n) With #primitive roots =
 *            φ(φ(n)) if n has primitive roots, otherwise 0. n has primitive
 *            roots iff n is 2,4, p^k, or 2·p^k for an odd prime p
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Utility: trial‑division smallest prime factor, used for all work.   */
/* ------------------------------------------------------------------ */
static uint64_t spf_uint64(uint64_t n)
{
    if (n <= 1) return 0;
    if (n % 2 == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return d;
    return n;          /* n itself is prime                                   */
}

/* ------------------------------------------------------------------ */
/* Utility: test whether m (>1) is an odd prime power.                */
/* ------------------------------------------------------------------ */
static bool is_odd_prime_power(uint64_t m)
{
    if (m <= 1 || (m & 1) == 0) return false;   /* need odd > 1             */
    uint64_t p = spf_uint64(m);
    while (m % p == 0) m /= p;
    return m == 1;
}

/* ------------------------------------------------------------------ */
/* Determine whether n possesses primitive roots.                    */
/* ------------------------------------------------------------------ */
static bool has_primitive_root(uint64_t n)
{
    if (n == 2 || n == 4) return true;
    if (n & 1U)           /* odd n */
        return is_odd_prime_power(n);
    if ((n & 3U) == 0)     /* divisible by 4 */
        return false;
    /* n is 2·odd, check odd part */
    return is_odd_prime_power(n >> 1);
}

/* ------------------------------------------------------------------ */
/* Compute Euler's totient φ(m) for 64‑bit integers.                  */
/* Returned value is cast into seq_t.                                 */
/* ------------------------------------------------------------------ */
static seq_t phi_uint64(uint64_t m)
{
    uint64_t result = m;
    uint64_t temp   = m;

    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            while (temp % p == 0) temp /= p;
            result = result / p * (p - 1);
        }
    }
    if (temp > 1) /* remaining prime factor */
        result = result / temp * (temp - 1);

    return (seq_t)result;
}

/* ------------------------------------------------------------------ */
/* Main OEIS sequence function.                                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0, base case             */

    uint64_t nn = (uint64_t)n;     /* work on a uint64_t form          */
    seq_t ans = (seq_t)(nn - 1);   /* start with n-1                  */

    if (has_primitive_root(nn))
    {
        uint64_t phi_n  = (uint64_t)phi_uint64(nn);           /* φ(n)   */
        uint64_t phi2_n = (uint64_t)phi_uint64(phi_n);        /* φ(φ(n))*/
        ans -= (seq_t)phi2_n;                                 /* subtract the number of primitive roots */
    }
    return ans;
}
