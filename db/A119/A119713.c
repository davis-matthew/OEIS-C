/*
 * A119713  First differences are 2, 5, 5, 9, 9, 9, 14, 14, 14, 14, ..., that
 *          is, A000096 with m-th term repeated m times (m>=1).
 * https://oeis.org/A119713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m(m+1)(m^2+5m+2)/8 + (n - m(m+1)/2) * (m+1)(m+4)/2 where
 *            m = floor((sqrt(8n+1)-1)/2) (m >= 0)
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(x)) for a 128‑bit integer x and return a u64.
   x will be at most about 8·2^63+1, so sqrt(x) < 2^34. */
static idx_t floor_sqrt_u128(unsigned __int128 x) {
    idx_t low = 0, high = ((idx_t)1) << 34;  /* 2^34 > sqrt(8·2^63+1) */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* m = floor((sqrt(8n+1)-1)/2) */
    unsigned __int128 tn = (unsigned __int128)8 * n + 1;
    idx_t s = floor_sqrt_u128(tn);   /* floor sqrt(8n+1) */
    idx_t m = (s - 1) / 2;

    /* Tm = m(m+1)/2 */
    unsigned __int128 Tm = (unsigned __int128)m * (m + 1) / 2;
    idx_t r = n - (idx_t)Tm;                 /* remaining terms in block m+1 */

    /* d_next = (m+1)(m+4)/2   (difference value for block m+1) */
    unsigned __int128 d_next = (unsigned __int128)(m + 1) * (m + 4) / 2;

    /* S_m = m(m+1)(m^2+5m+2)/8  (sum of full blocks up to m) */
    unsigned __int128 term1 = (unsigned __int128)m * (m + 1);
    unsigned __int128 term2 = (unsigned __int128)m * m + (unsigned __int128)5 * m + 2;
    unsigned __int128 sm = term1 * term2 / 8;

    /* total = S_m + r * d_next */
    unsigned __int128 part = (unsigned __int128)r * d_next;
    return (seq_t)sm + (seq_t)part;
}
