/*
 * A086025  a(n) = Sum_{i=1..n} C(i+4,5)^2.
 * https://oeis.org/A086025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} binomial(i+4,5)^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 19, u64 n <= 165, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#if defined(__cplusplus) && __cplusplus >= 201703L
#include <type_traits>
#endif

/* Helper to multiply by a small integer and divide by another while avoiding overflow.
   For built‑in integer seq_t (≤64 bits) it uses 128 bit intermediate.
   For arbitrary‑precision seq_t it relies on the type’s own operators. */
#ifndef __cplusplus
static inline seq_t mul_div(seq_t cur, unsigned long long mul, unsigned long long div)
{
    __uint128_t prod = (__uint128_t)cur * mul;
    prod /= div;
    return (seq_t)prod;
}
#else
static inline seq_t mul_div(seq_t cur, unsigned long long mul, unsigned long long div)
{
#if defined(__SIZEOF_INT128__)
    if constexpr (sizeof(seq_t) <= sizeof(unsigned long long)) {
        __uint128_t prod = (__uint128_t)cur * mul;
        prod /= div;
        return (seq_t)prod;
    } else {
        return (cur * mul) / div;
    }
#else
    // Fallback (unlikely for C++ builds that use __uint128_t)
    return (cur * mul) / div;
#endif
}
#endif

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)1;   /* C(5,5) */
    seq_t sum = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i) {
        /* sum += cur^2 */
        sum += cur * cur;

        if (i == n) {
            break;
        }

        /* next binomial: cur = cur * (i+5) / i */
        cur = mul_div(cur, (unsigned long long)(i + 5), (unsigned long long)i);
    }

    return sum;
}
