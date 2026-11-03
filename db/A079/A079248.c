/*
 * A079248  Sum of q in all pairs (p,q), 0 <= p < q, p+q divides n.
 * https://oeis.org/A079248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 347, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9765 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079248:  Sum of q in all pairs (p,q), 0 ≤ p < q, p+q|n.
   Formula: a(n) = Σ_{d|n} [ m*d - m*(m-1)/2 ], where
        m = ⌊(d-1)/2⌋ + 1   (number of pairs with sum d).  */

#include <stdint.h>

/* Helper: contribution of a single divisor d of n. */
static inline seq_t divisor_contrib(idx_t d) {
    if ((d & 1) == 0) {                /* d = 2k */
        idx_t k = d >> 1;
        return (seq_t)k * ((seq_t)(3 * k + 1)) / 2;
    } else {                           /* d = 2k+1 */
        idx_t k = d >> 1;              /* floor((d-1)/2) */
        return (seq_t)(k + 1) * ((seq_t)(3 * k + 2)) / 2;
    }
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t d;
    for (d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;
        /* divisor d */
        sum += divisor_contrib(d);
        /* paired divisor n/d, if distinct */
        idx_t d2 = n / d;
        if (d2 != d) sum += divisor_contrib(d2);
    }
    return sum;
}
