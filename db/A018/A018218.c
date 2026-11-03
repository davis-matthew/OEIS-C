/*
 * A018218  Sum(C(j)*(n-j)*4^(n-j-1),j=0..n-1), C = Catalan numbers.
 * https://oeis.org/A018218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(4^n - binomial(2n+1, n))/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 173 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
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
    /* 4^n */
    seq_t pow4n = pow4(n);
    /* binomial(2n+1, n) */
    seq_t bin = binomial(2 * n + 1, n);
    /* difference */
    seq_t diff = pow4n - bin;

    /* avoid overflow: divide first, using that
       either (n+1) or diff is even */
    if ((n + 1) & 1) {          /* n+1 is odd, diff is even */
        diff /= 2;
        return diff * (n + 1);
    } else {                     /* n+1 is even */
        return ((n + 1) / 2) * diff;
    }
}
