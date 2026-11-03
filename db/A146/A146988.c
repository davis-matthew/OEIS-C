/*
 * A146988  Triangle, read by rows, T(n, k) = binomial(n, k) for n < 2 and
 *          binomial(n, k) + 4^(n-1) * binomial(n-2, k-1) otherwise.
 * https://oeis.org/A146988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r and k are the row and column of the
 *            flattened triangle defined by T(n,k) = binomial(n,k) for n < 2
 *            T(n,k) = binomial(n,k) + 4^(n-1) * binomial(n-2,k-1) otherwise
 * Range:     u8 n <= 15, u16 n <= 37, u32 n <= 95, u64 n <= 308,
 *            u128 n <= 1097, big any n
 * Validated: standard, pass, 71 terms through n = 5108 (2026-11-01)
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

seq_t a(idx_t n) {
    /* Find row r such that the cumulative number of terms up to row r
       exceeds n.  The cumulative count up to row r-1 is r*(r+1)/2. */
    idx_t r = 0;
    idx_t cum = 0;          /* terms before current row */
    while (cum + r + 1 <= n) {
        cum += r + 1;
        r++;
    }
    idx_t k = n - cum;      /* column index within row r */

    /* For rows 0 and 1 we use ordinary binomial coefficients. */
    if (r < 2)
        return binomial(r, k);

    /* Compute T(r,k) for r >= 2 */
    seq_t term = binomial(r, k);

    if (k > 0) {
        seq_t add = binomial(r - 2, k - 1);
        if (add != 0) {
            seq_t power = pow4(r - 1);   /* 4^(r-1) */
            term += add * power;
        }
    }
    return term;
}
