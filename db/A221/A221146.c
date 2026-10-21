/*
 * A221146  Table read by antidiagonals: (m+n) - (m XOR n).
 * https://oeis.org/A221146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) = (m + k) - (m XOR k), where n = T_s + k, * T_s = s*(s+1)/2
 *            and 0 ≤ k ≤ s. The coordinates are * (m , k) with m = s – k.
 *            This yields the table read by * antidiagonals of (m+n)-(m XOR
 *            n). Complexity: O(log n)
 * Range:     u8 n <= 9614, u16 n <= 9614, u32 n <= 9614, u64 n <= 9614,
 *            u128 n <= 9614, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t triangular_index(idx_t n) {
    /* Find max s such that s*(s+1)/2 <= n */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n) low = mid;
        else high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n) {
    idx_t s   = triangular_index(n);                    /* diagonal index */
    unsigned __int128 T_s = (unsigned __int128)s * (s + 1) / 2;
    idx_t k = (idx_t)(n - T_s);                         /* offset on diagonal */
    idx_t m = s - k;                                     /* row index */

    /* Compute (m + k) - (m XOR k) in seq_t */
    seq_t sum = (seq_t)m + (seq_t)k;
    seq_t xr  = (seq_t)(m ^ k);
    return sum - xr;
}
