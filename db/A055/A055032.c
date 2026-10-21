/*
 * A055032  Denominator of (Sum(m^(n-1),m=1..n-1)+1)/n.
 * https://oeis.org/A055032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A055032: a(n) = denominator of ( Σ_{m=1}^{n-1} m^{n-1} + 1 ) / n */

#include <stdint.h>
#include "../utils/gcd_u64.h"

/* Auxiliary: modular exponentiation using 128‑bit intermediate arithmetic */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    uint64_t b   = base % mod;
    while (exp) {
        if (exp & 1U)
            res = (unsigned __int128)res * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1U;
    }
    return res;
}

/* Auxiliary: greatest common divisor for 64‑bit unsigned integers */

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;        /* work in native 64‑bit integers */

    if (N <= 1U)          /* a(1) = 1 by definition */
        return (seq_t)1;

    uint64_t exp = N - 1U;
    uint64_t sum = 0U;

    for (uint64_t m = 1U; m < N; ++m) {
        uint64_t term = mod_pow_u64(m, exp, N);
        sum += term;
        if (sum >= N) sum -= N;   /* keep sum modulo N */
    }

    uint64_t t = sum + 1U;
    if (t >= N) t -= N;           /* t = (sum + 1) mod N */

    uint64_t g = gcd_u64(t, N);
    uint64_t denom = N / g;

    return (seq_t)denom;
}
