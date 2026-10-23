/*
 * A185542  a(n) = m*(m+1)/2, where m = floor(n^(5/2)).
 * https://oeis.org/A185542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(5/2)) * (floor(n^(5/2)) + 1) / 2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 96, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isqrt_uint128_to_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses 128‑bit arithmetic to compute n^5 exactly and a binary search for the integer
   square root.  The final result is cast to seq_t.                         */

#include <stdint.h>
#include "../utils/isqrt_uint128_to_idx.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* a(0) undefined, return 0 */

    unsigned __int128 n128 = n;
    unsigned __int128 n5 = n128 * n128 * n128 * n128 * n128;   /* n^5 */

    idx_t m = isqrt_uint128_to_idx(n5);   /* floor sqrt(n^5) = floor(n^(5/2)) */

    seq_t m_seq = (seq_t)m;
    return m_seq * (m_seq + (seq_t)1) / (seq_t)2;
}
