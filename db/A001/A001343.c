/*
 * A001343  Number of (unordered) ways of making change for n cents using
 *          coins of 5, 10, 20, 50, 100 cents.
 * https://oeis.org/A001343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: 1/((1-x^5)*(1-x^10)*(1-x^20)*(1-x^50)*(1-x^100)). a(n) = 0
 *            unless 5|n; with m = n/5, count partitions of m into parts
 *            1,2,4,10,20 by enumerating the counts of 20s, 10s and 4s; the
 *            parts 1 and 2 then contribute floor(r/2)+1 ways for the
 *            remainder r
 * Range:     u8 n <= 199, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t total = 0;
    idx_t m, r20, r10, r4, k20, k10;
    if (n % 5 != 0) return total;
    m = n / 5;
    for (k20 = 0; k20 * 20 <= m; k20++) {
        r20 = m - k20 * 20;
        for (k10 = 0; k10 * 10 <= r20; k10++) {
            uint64_t sub = 0;
            r10 = r20 - k10 * 10;
            for (r4 = r10;; r4 -= 4) {
                sub += r4 / 2 + 1;
                if (r4 < 4) break;
            }
            total += (seq_t)sub;
        }
    }
    return total;
}
