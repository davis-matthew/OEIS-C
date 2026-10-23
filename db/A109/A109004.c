/*
 * A109004  Table of gcd(n, m) read by antidiagonals, n >= 0, m >= 0.
 * https://oeis.org/A109004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult, tabl
 * Formula:   a(n) = gcd(n_val, m_val), where (n_val,m_val) are derived from
 *            the antidiagonal indexing: Let s be the unique integer with
 *            s*(s+1)/2 ≤ n < (s+1)*(s+2)/2. offset = n - s*(s+1)/2; n_val = s
 *            - offset; m_val = offset. The sequence is the table of gcd(n,m)
 *            read by antidiagonals
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   my_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/my_gcd.h"
/* Simple Euclidean GCD for any seq_t type. */

seq_t a(idx_t n)
{
    /* Special case for the (0,0) entry. */
    if (n == 0) return (seq_t)0;

    /* Find the antidiagonal index s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
    idx_t low  = 0;
    idx_t high = 1;

    /* Expand high until its triangular number exceeds n. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    /* Binary search in [low, high-1]. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t s = low;

    /* Offset within the antidiagonal. */
    idx_t offset = n - (unsigned __int128)s * (s + 1) / 2;
    idx_t n_val = s - offset;
    idx_t m_val = offset;

    /* gcd(0,0) is defined as 0 in this sequence. */
    if (n_val == 0 && m_val == 0) return (seq_t)0;

    return my_gcd((seq_t)n_val, (seq_t)m_val);
}
