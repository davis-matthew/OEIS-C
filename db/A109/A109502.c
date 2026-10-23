/*
 * A109502  Array read by antidiagonals: T(m,n) is the number of closed walks
 *          of length n on the complete graph on m nodes, m >= 1, n >= 0.
 * https://oeis.org/A109502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = ((m-1)^k + (m-1)*(-1)^k)/m, where m = s - k, k = n -
 *            (s-1)*s/2 - 1, s = minimal integer with s*(s+1)/2 ≥ n (reading
 *            the array T(m,n) by antidiagonals)
 * Range:     u8 n <= 40, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_uint(idx_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)base;
    while (exp > 0) {
        if (exp & 1)
            result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find s such that s*(s+1)/2 >= n */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t s = lo;

    /* Compute offset k within the s-th antidiagonal */
    idx_t prev_tri = (s - 1) * s / 2;
    idx_t k = n - prev_tri - 1;    /* k ranges from 0 to s-1 */

    idx_t m = s - k;
    idx_t walk_len = k;            /* corresponds to n in T(m,n) */

    seq_t m_minus1 = (seq_t)(m - 1);
    seq_t pow_term = pow_uint((idx_t)m_minus1, walk_len);

    seq_t numerator;
    if ((walk_len & 1) == 0) {     /* even walk length */
        numerator = pow_term + m_minus1;
    } else {                       /* odd walk length */
        numerator = pow_term - m_minus1;
    }

    seq_t value = numerator / (seq_t)m;
    return value;
}
