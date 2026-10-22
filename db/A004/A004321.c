/*
 * A004321  Binomial coefficient C(3n, n-3).
 * https://oeis.org/A004321
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(3n, n-3) = Product_{i=1..n-3} (2n+3+i)/i
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   a004321_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004321_gcd_u64.h"

seq_t a(idx_t n)
{
    idx_t k = n - 3;
    idx_t base = 2 * n + 3;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        /* r * (base + i) / i is an integer; cancel gcd(r, i) first so the
           intermediate never exceeds the new value */
        uint64_t rm = (uint64_t)(r % (seq_t)i);
        uint64_t g = a004321_gcd_u64((uint64_t)i, rm);
        uint64_t d = (uint64_t)i / g;
        uint64_t q = (uint64_t)(base + i) / d;
        r = (r / (seq_t)g) * (seq_t)q;
    }
    return r;
}
