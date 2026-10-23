/*
 * A191494  Number of compositions of even natural numbers in 7 parts <= n.
 * https://oeis.org/A191494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n + 1)^7 + (1 + (-1)^n)/2 )/2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 25, u64 n <= 623, u128 n <= 1000,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

#if defined(__cplusplus)
#include <type_traits>

seq_t a(idx_t n)
{
    if constexpr (
        std::is_same_v<seq_t, uint64_t> ||
        std::is_same_v<seq_t, uint32_t> ||
        std::is_same_v<seq_t, uint16_t> ||
        std::is_same_v<seq_t, uint8_t>)
    {
        /* Use 128‑bit intermediate arithmetic for exactness. */
        using u128 = unsigned __int128;
        u128 b = (u128)(n + 1);
        u128 p = 1;
        for (int i = 0; i < 7; ++i) p *= b;
        u128 parity = ((u128)((n & 1ULL) ^ 1ULL));
        u128 r = (p + parity) >> 1;   /* division by 2 */
        return (seq_t)r;
    }
    else
    {
        /* Arbitrary‑precision seq_t: rely on the provided big_pow helper. */
        seq_t base = (seq_t)(n + 1);
        seq_t pow  = big_pow(base, 7);
        if ((n & 1ULL) == 0) pow += (seq_t)1;   /* (1+(-1)^n)/2 == 1 for even n */
        return pow >> 1;
    }
}
#else  /* C11 implementation – seq_t is an unsigned integer type */

typedef unsigned __int128 u128;

seq_t a(idx_t n)
{
    u128 b = (u128)(n + 1);
    u128 p = 1;
    for (int i = 0; i < 7; ++i) p *= b;
    u128 parity = ((n & 1ULL) ^ 1ULL);
    u128 r = (p + parity) >> 1;   /* integer division by 2 */
    return (seq_t)r;
}
#endif
