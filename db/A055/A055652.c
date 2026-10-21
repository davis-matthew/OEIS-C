/*
 * A055652  Table T(m,k)=m^k+k^m (with 0^0 taken to be 1) as square array read
 *          by antidiagonals.
 * https://oeis.org/A055652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(m,k) = m^k + k^m, where m + k = d, d = ⌊(√(8n+1)-1)/2⌋,
 *            m = n - d(d+1)/2, k = d - m, with 0^0 treated as 1
 * Range:     u8 n <= 38, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        if (exp > 1) base *= base;
        exp >>= 1;
    }
    return result;
}

static inline idx_t isqrt_u128(unsigned __int128 x) {
    idx_t low = 0;
    idx_t high = 1;
    while ((unsigned __int128)high * high <= x) high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n) {
    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d;
    {   /* Compute d via integer sqrt to avoid linear scan */
        unsigned __int128 val = (unsigned __int128)8 * n + 1;
        idx_t s = isqrt_u128(val);
        d = (s - 1) / 2;
    }
    /* Compute offset within the diagonal */
    idx_t prev = (unsigned __int128)d * (d + 1) / 2;
    idx_t offset = n - prev;
    idx_t m = offset;
    idx_t k = d - offset;
    /* Handle 0^0 = 1 automatically by pow_seq_t */
    seq_t term1 = pow_seq_t((seq_t)m, k);
    seq_t term2 = pow_seq_t((seq_t)k, m);
    return term1 + term2;
}
