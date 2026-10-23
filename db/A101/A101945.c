/*
 * A101945  a(n) = 6*2^n - n - 5.
 * https://oeis.org/A101945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*2^n - n - 5
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    if constexpr (std::is_integral<seq_t>::value)
    {
        seq_t pow2 = 1;
        for (idx_t i = 0; i < n; ++i) pow2 <<= 1;
        unsigned __int128 val = (unsigned __int128)pow2 * 6;
        val -= (unsigned __int128)n + 5;
        return (seq_t)val;
    }
    else
#endif
    {
        seq_t pow2 = 1;
        for (idx_t i = 0; i < n; ++i) pow2 *= 2;
        seq_t val = pow2 * 6;
        val -= (seq_t)(n + 5);
        return val;
    }
}
