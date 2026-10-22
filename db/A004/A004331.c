/*
 * A004331  Binomial coefficient C(4n,n-1).
 * https://oeis.org/A004331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(4n, n-1), computed as C(3n+1+i, i) for i = 1..n-1, with
 *            a gcd reduction so no intermediate value is larger than the
 *            result
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq.h"

seq_t a(idx_t n) {
    idx_t k = n - 1;
    idx_t base = 3 * n + 1; /* 4n - k */
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(base + i);
        seq_t den = (seq_t)i;
        seq_t g = gcd_seq(r, den);
        r /= g;
        den /= g;
        /* r*num/i is an integer and gcd(r, den) = 1, so den divides num */
        num /= den;
        r *= num;
    }
    return r;
}
