/*
 * A014306  a(n) = 0 if n of form m(m+1)(m+2)/6, otherwise 1.
 * https://oeis.org/A014306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n = m(m+1)(m+2)/6 for some m >= 0, otherwise 1
 * Range:     u8 n <= 119, u16 n <= 7561, u32 n <= 7561, u64 n <= 7561,
 *            u128 n <= 7561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the m-th tetrahedral number: binomial(m+2, 3).  */
static seq_t tetrahedral(idx_t m) {
    unsigned __int128 p = (unsigned __int128)m * (m + 1);
    p *= (m + 2);
    p /= 6;
    return (seq_t)p;
}

seq_t a(idx_t n) {
    /* Quick check for the smallest tetrahedral values. */
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 4) return 0;
    if (n == 10) return 0;
    if (n == 20) return 0;
    /* General case: binary search for a tetrahedral number. */
    idx_t low = 0, high = 1;
    /* Expand high until tetrahedral(high) > n. */
    while (tetrahedral(high) <= n) {
        high <<= 1;
        /* Avoid overflow: high will never exceed about 5e6 for uint64_t n. */
    }
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t v = tetrahedral(mid);
        if (v == n)
            return 0;
        else if (v < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return 1;
}
