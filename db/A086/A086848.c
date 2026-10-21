/*
 * A086848  First differences are formed by interleaving {2^k - 1, k = 1, 2,
 *          3, ...} and {11*k, k = 5, 4, 3, ...}.
 * https://oeis.org/A086848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 42 For n > 1: a(n) = a(n-1) + d(n-1) d(i) = 2^k - 1 where
 *            k = (i+1)/2 if i is odd = 11*(6 - k') where k' = i/2 if i is
 *            even (After k' > 6 the even differences become negative.) This
 *            implementation computes the sum incrementally in O(n)
 * Range:     u8 n <= 10, u16 n <= 31, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)42;
    __int128 total = 42;      /* use signed to accommodate negative gaps */
    seq_t power = 2;          /* 2^1 for the first odd difference */
    for (idx_t i = 1; i < n; ++i) {
        if (i & 1) {                 /* odd index  -> 2^k - 1 */
            __int128 diff = (__int128)power - 1;
            total += diff;
            power <<= 1;             /* next odd will need 2^(k+1) */
        } else {                     /* even index -> 11*(6 - k') */
            idx_t k_half = i / 2;
            int64_t val = 6 - (int64_t)k_half;   /* can be negative */
            __int128 diff = (__int128)11 * val;
            total += diff;
        }
    }
    return (seq_t)total;
}
