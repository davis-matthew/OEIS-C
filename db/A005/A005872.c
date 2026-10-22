/*
 * A005872  Theta series of hexagonal close-packing with respect to octahedral
 *          hole.
 * https://oeis.org/A005872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * Sum_{j odd, j^2 < n, m=(n-j^2)/2, m == 1 (mod 3)}
 *            3*Sum_{d|m} chi_{-3}(d), i.e. n = (2k+1)^2 + 2m over layers k
 *            and in-plane A2-coset norms m (a(2n) = 0)
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   eis_divsum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/eis_divsum.h"

seq_t a(idx_t n) {
    if (n % 2 == 0) return (seq_t)0;
    uint64_t total = 0;
    uint64_t j;
    for (j = 1; j * j < (uint64_t)n; j += 2) {
        uint64_t m = ((uint64_t)n - j * j) / 2;
        if (m % 3 != 1) continue;
        int64_t e = eis_divsum(m);
        total += (uint64_t)(6 * e);
    }
    return (seq_t)total;
}
