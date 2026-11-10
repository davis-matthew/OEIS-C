/*
 * A077584  Last term of n-th row of A077583.
 * https://oeis.org/A077584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer k >= n with gcd(k,n) > 1 (a(1)=1); computed
 *            by counting non-coprime residues mod n per period
 * Range:     u8 n <= 16, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A077584_gcd(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    idx_t m = 0;
    idx_t r;
    for (r = 0; r < n; ++r)
        if (A077584_gcd(n, r) > 1) ++m;
    idx_t period = (n - 1) / m;
    idx_t rem = (n - 1) % m;
    idx_t c = 0;
    idx_t pos = 0;
    for (r = 0; r < n; ++r) {
        if (A077584_gcd(n, r) > 1) {
            if (c == rem) { pos = r; break; }
            ++c;
        }
    }
    return (seq_t)n + (seq_t)period * (seq_t)n + (seq_t)pos;
}
