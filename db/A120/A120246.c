/*
 * A120246  a(1) = 1. a(m(m+1)/2 + k) = m + a(k), 1 <= k <= m+1, m >= 1.
 * https://oeis.org/A120246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1; for n = m(m+1)/2 + k (1 ≤ k ≤ m+1) a(n) = m + a(k)
 * Range:     u8 n <= 9175, u16 n <= 9175, u32 n <= 9175, u64 n <= 9175,
 *            u128 n <= 9175, big any n
 * Helpers:   triangular_upper_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers needed for types and sqrtl. */
#include <stdint.h>
#include <math.h>
#include "../utils/triangular_upper_idx.h"

/* Main function implementing the recurrence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* not defined for 0, safe fallback */

    seq_t result = 1;                  /* base: a(1) = 1 */
    idx_t cur = n;

    while (cur > 1) {
        idx_t m = triangular_upper_idx(cur);
        idx_t k = cur - (idx_t)((unsigned __int128)m * (m + 1) / 2);
        result += (seq_t)m;
        cur = k;
    }
    return result;
}
