/*
 * A018217  a(n) = Sum_{j=0..n-1} C(j)*(n-j)*4^(n-j), C = Catalan numbers.
 * https://oeis.org/A018217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n+1)*(4^n - binom(2*n+1, n))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 986 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t nn          = (seq_t)n;
    seq_t pow4_val    = pow4(n);
    seq_t bin_val     = binomial(2 * n + 1, n);
    seq_t diff        = pow4_val - bin_val;

    seq_t res = 2;
    res *= (nn + 1);
    res *= diff;
    return res;
}
