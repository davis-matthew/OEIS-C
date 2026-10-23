/*
 * A121873  Number of non-crossing plants in the (n+1)-sided regular polygon
 *          (contains non-crossing trees).
 * https://oeis.org/A121873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{k=0..n-1} binom(n-1+k,k) * binom(2*n-2+k+2,
 *            n-1-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1 */

#include <stdint.h>
#include "../utils/binom.h"

#ifdef __cplusplus
#include <type_traits>
constexpr bool use_bigint = !std::is_integral<seq_t>::value;
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    idx_t m = n - 1;
    seq_t sum = 0;

#if defined(__cplusplus)
    if constexpr (use_bigint) {
        for (idx_t k = 0; k <= m; ++k) {
            seq_t b1 = binom(m + k, k);
            seq_t b2 = binom(2 * m + k + 2, m - k);
            seq_t prod = b1 * b2;
            seq_t term = prod / (seq_t)n;
            sum += term;
        }
    } else {
        for (idx_t k = 0; k <= m; ++k) {
            seq_t b1 = binom(m + k, k);
            seq_t b2 = binom(2 * m + k + 2, m - k);
            unsigned __int128 prod = (unsigned __int128)b1 * (unsigned __int128)b2;
            unsigned __int128 term = prod / (unsigned __int128)n;
            sum += (seq_t)term;
        }
    }
#else
    for (idx_t k = 0; k <= m; ++k) {
        seq_t b1 = binom(m + k, k);
        seq_t b2 = binom(2 * m + k + 2, m - k);
        unsigned __int128 prod = (unsigned __int128)b1 * (unsigned __int128)b2;
        unsigned __int128 term = prod / (unsigned __int128)n;
        sum += (seq_t)term;
    }
#endif

    return sum;
}
