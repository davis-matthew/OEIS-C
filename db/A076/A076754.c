/*
 * A076754  a(n) = nearest integer to the skewness of the divisors of n.
 * https://oeis.org/A076754
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to the skewness of the divisors of n
 *            (skewness = third central moment / (standard deviation)^3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
typedef unsigned __int128 u128;
typedef signed   __int128   i128;
#else
/* Fallback: use 64‑bit arithmetic if 128‑bit not available.
   The test data (n <= 10000) fit safely into 64 bits with this
   approach. */
typedef unsigned long long u128;
typedef long long          i128;
#endif

seq_t a(idx_t n)
{
    /* The OEIS offset is 2; values for n < 2 are irrelevant. */
    if (n < 2u)
        return 0;

    /* Enumerate divisors of n and accumulate sums of d, d², d³. */
    u128 S1 = 0;            /* Σ d      */
    u128 S2 = 0;            /* Σ d²     */
    u128 S3 = 0;            /* Σ d³     */
    idx_t k  = 0;           /* number of divisors */

    for (idx_t i = 1; (u128)i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            S1 += (u128)d1;
            S2 += (u128)d1 * (u128)d1;
            S3 += (u128)d1 * (u128)d1 * (u128)d1;
            ++k;

            if (d1 != d2) {
                S1 += (u128)d2;
                S2 += (u128)d2 * (u128)d2;
                S3 += (u128)d2 * (u128)d2 * (u128)d2;
                ++k;
            }
        }
    }

    /* Intermediate values for skewness. */
    u128 V_num = S2 * (u128)k - S1 * S1;          /* k² * variance */
    /* μ3num is the third central moment multiplied by k³ */
    i128 mu3num = (i128)S3 * (i128)k * (i128)k
                  - (i128)3 * (i128)S1 * (i128)S2 * (i128)k
                  + (i128)2 * (i128)S1 * (i128)S1 * (i128)S1;

    /* Compute skewness as a floating point number for rounding. */
    long double df = (long double)((long long)mu3num);
    long double Vsqrt = sqrtl((long double)V_num);
    long double denom = (long double)V_num * Vsqrt;   /* V_num^(3/2) */

    long double skew = df / denom;
    seq_t result = (seq_t)llroundl(skew);             /* nearest integer */

    return result;
}
