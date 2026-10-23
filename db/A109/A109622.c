/*
 * A109622  Number of different isotemporal classes of diasters with n
 *          peripheral edges.
 * https://oeis.org/A109622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1; for n > 0,
 * if n is odd (n = 2k+1):  a(n) = 1 + Σ_{i=1..k} (n*i - i^2 + n + 1)
 * if n is even (n = 2k):   a(n) = 1 + Σ_{i=1..k-1} (n*i - i^2 + n + 1)
 *                           + ½ * (k^2 + 3k + 2)
 */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t n_seq = (seq_t)n;
    if ((n & 1ULL) != 0) { /* odd */
        idx_t k = (n - 1) / 2;
        seq_t sum = 0;
        for (idx_t i = 1; i <= k; ++i) {
            seq_t ii = (seq_t)i;
            sum += n_seq * ii - ii * ii + n_seq + 1;
        }
        return 1 + sum;
    } else { /* even */
        idx_t k = n / 2;
        seq_t sum = 0;
        for (idx_t i = 1; i < k; ++i) {
            seq_t ii = (seq_t)i;
            sum += n_seq * ii - ii * ii + n_seq + 1;
        }
        seq_t k_seq = (seq_t)k;
        seq_t half = (k_seq * k_seq + 3 * k_seq + 2) / 2;
        return 1 + sum + half;
    }
}
