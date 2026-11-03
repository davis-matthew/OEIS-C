/*
 * A004390  Binomial coefficient C(8n,n-9).
 * https://oeis.org/A004390
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(8*n, n-9), computed multiplicatively as
 *            prod_{i=1..n-9} (7*n+9+i)/i with exact division at each step
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 15, u64 n <= 22, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 26 (2026-11-01)
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_idx.h"

seq_t a(idx_t n)
{
    idx_t m = 8 * n;
    idx_t k = n - 9;
    idx_t base = m - k;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        idx_t rm = (idx_t)(r % (seq_t)i);
        idx_t g = gcd_idx(i, rm);
        idx_t den = i / g;
        idx_t t = (base + i) / den;
        r = (r / (seq_t)g) * (seq_t)t;
    }
    return r;
}
