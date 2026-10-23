/*
 * A125119  Values of repunits with odd length L in base (L+3)/2
 *          representation.
 * https://oeis.org/A125119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum((n+1)^k: 0<=k<=2*(n-1)) = ((n+1)^(2n-1)-1)/n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
#ifndef __cplusplus
    /* C implementation – safe for integral seq_t using 128‑bit arithmetic */
    if (n == 0) return 0;                    /* unused – offset 1 */
    __uint128_t base = ( __uint128_t )n + 1;
    __uint128_t power = 1;
    for (idx_t i = 0; i < 2 * n - 1; ++i)
        power *= base;
    power -= 1;                                 /* (n+1)^(2n-1) – 1 */
    power /= n;                                 /* divide by n */
    return (seq_t)power;
#else
    /* C++ implementation – use 128‑bit path for arithmetic seq_t,
       otherwise use the arbitrary‑precision helper seq_pow. */
    if constexpr (std::is_arithmetic_v<seq_t>) {
        __uint128_t base = ( __uint128_t )n + 1;
        __uint128_t power = 1;
        for (idx_t i = 0; i < 2 * n - 1; ++i)
            power *= base;
        power -= 1;
        power /= n;
        return (seq_t)power;
    } else {
        seq_t base = (seq_t)n + 1;                      /* >=2 for n>=1 */
        seq_t power = seq_pow(base, (idx_t)(2 * n - 1));
        power = power - 1;
        return power / (seq_t)n;
    }
#endif
}
