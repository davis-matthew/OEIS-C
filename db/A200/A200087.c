/*
 * A200087  Smallest number m such that A079878(m) = n.
 * https://oeis.org/A200087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m with A079878(m) = n, where A079878(1) = 1 and
 *            A079878(k) = 2*A079878(k-1) if 2*A079878(k-1) <= k, else
 *            2*A079878(k-1) - k. Computed by direct search (simulating the
 *            recurrence)
 * Range:     u8 n <= 26, u16 n <= 36, u32 n <= 860, u64 n <= 860,
 *            u128 n <= 860, big any n
 * Validated: standard, pass, 76 terms through n = 849 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t v = 1;
    uint64_t m = 1;
    const uint64_t limit = 1000000000ULL;
    if (n < 1) return 0;
    if ((uint64_t)n == 1) return (seq_t)1;
    while (m < limit) {
        m++;
        v = v + v;
        if (v > m) v -= m;
        if (v == (uint64_t)n) return (seq_t)m;
    }
    return 0;
}
