/*
 * A007062  Let P(n) of a sequence s(1),s(2),s(3),... be obtained by leaving
 *          s(1),...,s(n) fixed and reversing every n consecutive terms
 *          thereafter; apply P(2) to 1,2,3,... to get PS(2), then apply P(3)
 *          to PS(2) to get PS(3), then apply P(4) to PS(3), etc. This
 *          sequence is the limit of PS(n).
 * https://oeis.org/A007062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the original position (value) that lands at index n
 *            after applying P(2),...,P(n-1); computed by tracing index n
 *            backwards through each block reversal: for m = n-1 down to 2, if
 *            p > m then p -> m+1 + m*floor((p-m-1)/m) + (m-1 - ((p-m-1) mod
 *            m)). a(1) = 1
 * Range:     u8 n <= 32, u16 n <= 535, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t p = (uint64_t)n;
    if (n <= 2) return (seq_t)p;
    for (uint64_t m = (uint64_t)n - 1; m >= 2; m--) {
        if (p > m) {
            uint64_t q = p - m - 1;
            uint64_t k = q / m;
            uint64_t r = q % m;
            p = m + 1 + k * m + (m - 1 - r);
        }
    }
    return (seq_t)p;
}
