/*
 * A081460  Consider the mapping f(r) = (1/2)*(r + N/r) from rationals to
 *          rationals where N = 5. Starting with a = 2 and applying the
 *          mapping to each new (reduced) rational number gives 2, 9/4,
 *          161/72, 51841/23184, ..., tending to N^(1/2). Sequence gives
 *          values of the denominators.
 * https://oeis.org/A081460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the denominator of the n‑th term obtained by
 *            iterating the mapping f(r) = (r + 5/r) / 2, starting from r = 2.
 *            The first term is a(1) = 1 (the denominator of 2/1). Each
 *            subsequent term is reduced to lowest terms before the next
 *            iteration. The algorithm uses the recurrence p' = (p² + 5q...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   gcd_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq_t.h"

seq_t a(idx_t n) {
    /* n ≥ 1 by specification; behaviour for n < 1 is irrelevant */
    seq_t p = 2;   /* numerator */
    seq_t q = 1;   /* denominator – a(1) */

    for (idx_t i = 1; i < n; ++i) {
        seq_t newP = p * p + (seq_t)5 * q * q;
        seq_t newQ = (seq_t)2 * p * q;
        seq_t g = gcd_seq_t(newP, newQ);   /* reduce to lowest terms */
        newP /= g;
        newQ /= g;
        p = newP;
        q = newQ;
    }
    return q;
}
