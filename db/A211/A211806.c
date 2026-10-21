/*
 * A211806  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2w^2<=x^2+y^2.
 * https://oeis.org/A211806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A211806: Number of ordered triples (w,x,y) with 1≤w,x,y≤n
   and 2*w^2 ≤ x^2 + y^2. */
#include <stdint.h>

/* Integer square‑root for unsigned 64‑bit values (floor). */
static inline idx_t isqrt_u64(idx_t n) {
    if (n == 0)
        return 0;
    idx_t x = n, y = (x + 1) >> 1;   /* better initial guess than (x>>1)+1 */
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t count = 0;                     /* accumulator */
    for (idx_t x = 1; x <= n; ++x) {
        idx_t xx = x * x;                /* x² */
        for (idx_t y = 1; y <= n; ++y) {
            idx_t yy = y * y;            /* y² */
            idx_t sum = xx + yy;         /* x² + y² */
            idx_t over2 = sum >> 1;      /* floor((x² + y²)/2) */
            idx_t wmax = isqrt_u64(over2); /* floor(√over2) */
            count += (seq_t)wmax;
        }
    }
    return count;
}
