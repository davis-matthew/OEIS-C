/*
 * A161937  The number of indirect isometries that are derangements of the
 *          (n-1)-dimensional facets of an n-cube.
 * https://oeis.org/A161937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*n*a(n-1) + (-1)^{n-1}*n   for n ≥ 2,
 *            a(1) = 1
 * This recurrence follows from the e.g.f. F(x) = x*exp(-x)/(1-2x).
 */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1, but handle safely */
    if (n == 1) return 1;

    seq_t prev = 1;                /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = ((seq_t)2 * (seq_t)i) * prev;   /* 2*i*prev */
        if ((i & 1) == 0)          /* even i → subtract i */
            prev = tmp - (seq_t)i;
        else                       /* odd i  → add i   */
            prev = tmp + (seq_t)i;
    }
    return prev;
}
