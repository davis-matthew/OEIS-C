/*
 * A216702  a(n) = Product_{k=1..n} (16 - 4/k).
 * https://oeis.org/A216702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} (16 - 4/k). Recurrence: a(n) = a(n-1) *
 *            (4*(4*n-1)/n), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#if defined(__cplusplus) && (__cplusplus >= 201703L)
#include <type_traits>
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus) && (__cplusplus >= 201703L)
    if constexpr (std::is_arithmetic<seq_t>::value) {
        seq_t res = 1;
        for (idx_t i = 1; i <= n; ++i) {
            unsigned __int128 temp = (unsigned __int128)res *
                                    (unsigned __int128)(4 * (4 * i - 1));
            res = (seq_t)(temp / i);
        }
        return res;
    } else {
        seq_t res = 1;
        for (idx_t i = 1; i <= n; ++i) {
            res = (res * (seq_t)(4 * (4 * i - 1))) / i;
        }
        return res;
    }
#else
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        unsigned __int128 temp = (unsigned __int128)res *
                                (unsigned __int128)(4 * (4 * i - 1));
        res = (seq_t)(temp / i);
    }
    return res;
#endif
}
