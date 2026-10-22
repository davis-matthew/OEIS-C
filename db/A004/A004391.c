/*
 * A004391  Binomial coefficient C(8n,n-10).
 * https://oeis.org/A004391
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(8*n, n-10)
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 15, u64 n <= 22, u128 n <= 27,
 *            big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n) {
    idx_t m = 8 * n;
    idx_t k = n - 10;
    seq_t r = 1;
    idx_t i;
    /* r = C(m-k+i, i) after step i; each step is exact and grows monotonically */
    for (i = 1; i <= k; i++) {
        idx_t num = m - k + i;
        uint64_t rm = (uint64_t)(r % (seq_t)i);
        uint64_t g = gcd_u64(rm, (uint64_t)i);
        idx_t d = i / g;
        r = (r / (seq_t)g) * (seq_t)(num / d);
    }
    return r;
}
