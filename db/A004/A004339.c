/*
 * A004339  Binomial coefficient C(4n,n-9).
 * https://oeis.org/A004339
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n, n-9)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 16, u64 n <= 26, u128 n <= 45,
 *            big any n
 * Helpers:   a004339_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004339_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 9;
    idx_t base = 3 * n + 9;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        uint64_t x = (uint64_t)(base + i);
        uint64_t rm = (uint64_t)(r % (seq_t)i);
        uint64_t g = a004339_gcd(rm, (uint64_t)i);
        uint64_t d = (uint64_t)i / g;
        r = (r / (seq_t)g) * (seq_t)(x / d);
    }
    return r;
}
