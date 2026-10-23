/*
 * A033806  Product t2(q^d); d | 47, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of positive odd integer solutions (x,y) to x² + 47 y² =
 *            8 (n + 6)
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"
#include "../utils/seq_mul.h"

/* External linkage */
seq_t a(idx_t n)
{
    /* constants */
    const seq_t eight      = (seq_t)8;
    const seq_t fortyseven = (seq_t)47;
    const seq_t offset     = (seq_t)6;

    /* target value 8(n + 6) */
    const seq_t target = eight * (((seq_t)n) + offset);

    /* maximum possible y satisfies 47*y² < target
       => y² < target / 47  */
    const seq_t max_y_sq = target / fortyseven;
    const seq_t max_y    = isqrt_seq(max_y_sq);

    seq_t count = (seq_t)0;

    /* iterate over odd positive y */
    for (idx_t y = 1; y <= (idx_t)max_y; y += 2) {
        const seq_t y2   = seq_mul((seq_t)y, (seq_t)y);          /* y²          */
        const seq_t sub  = seq_mul(y2, fortyseven);             /* 47*y²       */
        const seq_t t    = target - sub;                        /* x²          */

        /* t must be positive; we know sub ≤ target from the loop bound */
        if (t == (seq_t)0) continue;      /* x would be 0 – not a positive solution */

        const seq_t r = isqrt_seq(t);                     /* floor sqrt(t) */
        /* Check perfect square and odd x */
        if (seq_mul(r, r) == t && (r & (seq_t)1) && r > (seq_t)0) {
            ++count;
        }
    }

    return count;
}
