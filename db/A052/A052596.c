/*
 * A052596  E.g.f. (1+x^4-x^5)/(1-x).
 * https://oeis.org/A052596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A052596
 * OEIS formula: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=48,
 *          a(5)=120, and for n≥6 the recurrence a(n)=n·a(n−1)
 *          (i.e. a(n)=n! for all n≠4, with a(4)=48).
 *
 * This is equivalent to the exponential‑generating function
 * (1+x⁴−x⁵)/(1−x).
 * The implementation simply checks the first few special cases
 * and otherwise returns n! computed by a straightforward product.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* pre‑computed small values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)48;      /* special case: 48 instead of 24 */

    /* for n ≥ 5 compute n! iteratively (this also gives a(5)=120) */
    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        prod *= (seq_t)i;
    }
    return prod;
}
