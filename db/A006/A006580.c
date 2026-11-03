/*
 * A006580  a(n) = Sum_{k=1..n-1} lcm(k,n-k).
 * https://oeis.org/A006580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} lcm(k,n-k)
 * Range:     u8 n <= 12, u16 n <= 78, u32 n <= 2956, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9848 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_u64.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t k;
    if (n <= 1) return 0;
    for (k = 1; k < n; k++) {
        uint64_t m = (uint64_t)(n - k);
        uint64_t g = gcd_u64((uint64_t)k, m);
        seq_t lcm_val = (seq_t)((uint64_t)k / g) * (seq_t)m;
        sum += lcm_val;
    }
    return sum;
}
