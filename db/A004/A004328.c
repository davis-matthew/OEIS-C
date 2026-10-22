/*
 * A004328  Binomial coefficient C(3n,n-10).
 * https://oeis.org/A004328
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(3n, n-10), computed multiplicatively as
 *            prod_{i=1..n-10} (2n+10+i)/i
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 18, u64 n <= 29, u128 n <= 52,
 *            big any n
 * Helpers:   a004328_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004328_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 10;
    idx_t base = 2 * n + 10;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(base + i);
        seq_t den = (seq_t)i;
        seq_t g = a004328_gcd(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
