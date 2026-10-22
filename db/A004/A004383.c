/*
 * A004383  Binomial coefficient C(8n,n-2).
 * https://oeis.org/A004383
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(8*n, n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   a004383_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004383_gcd.h"

seq_t a(idx_t n) {
    idx_t m = 8 * n;
    idx_t k = n - 2;
    seq_t r = 1;
    idx_t i;
    for (i = 0; i < k; i++) {
        /* r <- r * (m - i) / (i + 1), kept exact without overflow */
        seq_t num = (seq_t)(m - i);
        seq_t den = (seq_t)(i + 1);
        seq_t g = a004383_gcd(r, den);
        seq_t rr = r / g;
        seq_t d = den / g;
        r = rr * (num / d);
    }
    return r;
}
