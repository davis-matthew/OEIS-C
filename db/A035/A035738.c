/*
 * A035738  Coordination sequence for 43-dimensional cubic lattice.
 * https://oeis.org/A035738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   n*a(n) = 86*a(n-1) + (n-2)*a(n-2), a(0)=1, a(1)=86
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 18, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)86;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)86;  /* a(1) */
    seq_t a_cur;                /* will hold a(i) */

    for (idx_t i = 2; i <= n; ++i) {
#ifdef __cplusplus
        if constexpr (std::is_integral<seq_t>::value) {
            using u128 = unsigned __int128;
            a_cur = (seq_t)(((u128)86 * a_prev1
                               + (u128)(i - 2) * a_prev2) / (u128)i);
        } else {
            a_cur = (86 * a_prev1 + (i - 2) * a_prev2) / i;
        }
#else
        unsigned __int128 term1 = (unsigned __int128)86 * a_prev1;
        unsigned __int128 term2 = (unsigned __int128)(i - 2) * a_prev2;
        unsigned __int128 num   = term1 + term2;
        a_cur = (seq_t)(num / (unsigned __int128)i);
#endif
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_cur;
}
