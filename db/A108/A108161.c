/*
 * A108161  Partial sums of the positive integers n according to the rule: if
 *          n is square then add sqrt(n) else add n.
 * https://oeis.org/A108161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)/2 -
 *            floor(sqrt(n))*(floor(sqrt(n))+1)*(floor(sqrt(n))-1)/3
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t isqrt_u64(uint64_t n) {
    if (n < 2) return n;
    uint64_t low = 1, high = n >> 1;
    while (low <= high) {
        uint64_t mid = low + (high - low) / 2;
        if (mid <= n / mid) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n) {
    const seq_t sn = (seq_t)n;
    seq_t total = sn * (sn + 1) / 2;

    idx_t k = isqrt_u64(n);
    seq_t sk = (seq_t)k;
    seq_t delta = sk * (sk + 1) * (sk - 1) / 3;

    return total - delta;
}
