/*
 * A005317  a(n) = (2^n + C(2*n,n))/2.
 * https://oeis.org/A005317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) + C(2*n-1, n-1) for n >= 1, a(0) = 1 (equivalent
 *            to a(n) = (2^n + C(2*n,n))/2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   a005317_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a005317_gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* 2^(n-1) */
    seq_t pow2 = 1;
    for (idx_t i = 1; i < n; i++) pow2 *= 2;

    /* C(2n-1, n-1) = prod_{i=1}^{n-1} (n+i)/i, computed with gcd reduction
       so intermediates never exceed the final binomial */
    seq_t r = 1;
    for (idx_t i = 1; i < n; i++) {
        seq_t si = (seq_t)i;
        seq_t g = a005317_gcd(r, si);
        r /= g;
        seq_t num = ((seq_t)n + si) / (si / g);
        r *= num;
    }

    return pow2 + r;
}
