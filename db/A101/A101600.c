/*
 * A101600  Expansion of g.f. c(3x)^2, where c(x) is the g.f. of A000108.
 * https://oeis.org/A101600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n * Catalan(n+1) Recurrence: a(0) = 1 a(n) =
 *            6*(2*n+1)/(n+2) * a(n-1) for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* start with a(0) = 1 */
    seq_t res = 1;
    if (n == 0) return res;

    for (idx_t i = 1; i <= n; ++i) {
        /* numerator = 6 * (2*i + 1)  (fits in 64 bits for n <= 10^4) */
        uint64_t num   = 6ULL * (2ULL * i + 1ULL);
        uint64_t den   = i + 2;                    /* den ≤ 10002, fits in 64 bits */

        /* Reduce gcd between current result and denominator first */
        /* For builtin small types we can use 128‑bit multiplication safely */
#if defined(__SIZEOF_INT128__) && !defined(__INTEL_COMPILER)
        /* Use 128‑bit intermediate to avoid overflow */
        __uint128_t tmp = (__uint128_t)res * (uint64_t)num;
        res = (seq_t)(tmp / den);
#else
        /* Fallback: use seq_t arithmetic directly (works for arbitrary precision types) */
        res = res * (seq_t)num / (seq_t)den;
#endif
    }
    return res;
}
