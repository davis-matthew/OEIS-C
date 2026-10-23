/*
 * A181940  a(0)=0, and there are a(n) terms between a(n) and the nearest
 *          a(n)+1.
 * https://oeis.org/A181940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m - 2*pos (if pos <= floor(m/2) or pos <= k) else (even
 *            m): a(n) = 2*(pos - k) - 1 else (odd m): a(n) = if pos==k+1 ? 0
 *            : 2*(pos - (k+1)) where m = ⌊(√(8n+1)-1)/2⌋, T(m)=m*(m+1)/2 and
 *            pos = n - T(m)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Find m such that T(m) = m*(m+1)/2 <= n < T(m+1) */
    idx_t low = 0, high = 1;
    /* Increase high until T(high) > n */
    while (( __int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    idx_t m = low - 1;  /* largest m with T(m) <= n */

    /* triangular number T(m) */
    seq_t mt = (seq_t)m;
    seq_t Tseq = mt * (mt + 1) / 2;
    idx_t T_idx = (idx_t)Tseq;

    idx_t pos = n - T_idx;  /* position within block 0..m */

    /* Compute a(n) according to parity of m */
    if ((m & 1) == 0) {          /* m even: m = 2k */
        idx_t k = m >> 1;
        if (pos <= k) {
            return (seq_t)(m - 2 * pos);
        } else {
            return (seq_t)(2 * (pos - k) - 1);
        }
    } else {                     /* m odd: m = 2k + 1 */
        idx_t k = m >> 1;        /* floor(m/2) */
        if (pos <= k) {
            return (seq_t)(m - 2 * pos);
        } else if (pos == k + 1) {
            return (seq_t)0;
        } else {
            return (seq_t)(2 * (pos - (k + 1)));
        }
    }
}
