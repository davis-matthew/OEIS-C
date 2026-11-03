/*
 * A092531  Expansion of (1+x^10)/((1-x)*(1-x^4)^2*(1-x^8)).
 * https://oeis.org/A092531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{b=0}^{⌊n/8⌋} (⌊(n-8b)/4⌋+1)(⌊(n-8b)/4⌋+2)/2 + (same
 *            expression evaluated at n-10, if n ≥ 10)
 * Range:     u8 n <= 41, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t tsum(idx_t n)
{
    seq_t sum = 0;
    idx_t maxb = n / 8;              /* largest b with 8b ≤ n   */
    for (idx_t b = 0; b <= maxb; ++b) {
        idx_t rem = n - 8 * b;        /* remaining amount        */
        idx_t A   = rem / 4;          /* floor((n-8b)/4)         */
        seq_t term = ((seq_t)(A + 1) * (A + 2)) / 2; /* (A+1)(A+2)/2      */
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t res = tsum(n);
    if (n >= 10) {
        res += tsum(n - 10);
    }
    return res;
}
