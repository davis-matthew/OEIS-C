/*
 * A163455  a(n) = binomial(5*n-1,n).
 * https://oeis.org/A163455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(5*n-1, n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
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
    if (n == 0u) {
        return (seq_t)1;
    }

    /* top = 5*n - 1, k = n */
    idx_t top = 5 * n - 1;
    idx_t k   = n;
    seq_t result = 1;

#ifdef __cplusplus
    /* C++11/17: choose path based on whether seq_t is an integral type */
    if constexpr (std::is_integral<seq_t>::value) {
        for (idx_t i = 1; i <= k; ++i) {
            idx_t num = top - k + i;          /* numerator = 4*n - 1 + i */
            unsigned __int128 temp = (unsigned __int128)result * num;
            temp /= i;
            result = (seq_t)temp;
        }
    } else {
        /* seq_t is an arbitrary-precision integer class */
        for (idx_t i = 1; i <= k; ++i) {
            idx_t num = top - k + i;
            result = (result * (seq_t)num) / (seq_t)i;
        }
    }
#else
    /* C: seq_t is an unsigned integer type – use 128‑bit intermediate */
    for (idx_t i = 1; i <= k; ++i) {
        idx_t num = top - k + i;
        unsigned __int128 temp = (unsigned __int128)result * num;
        temp /= i;
        result = (seq_t)temp;
    }
#endif

    return result;
}
