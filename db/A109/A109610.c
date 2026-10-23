/*
 * A109610  Expansion of
 *          (1+3*x^4-2*x^7+x^10-x^12)/((x+1)*(x^2+1)*(x^2+x+1)*(x^2-x+1)*(x^4-x^2+1)*(x-1)^2).
 * https://oeis.org/A109610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + a(n-12) - a(n-13), with initial values:
 * a(0)=1, a(1)=1, a(2)=1, a(3)=1,
 * a(4)=4, a(5)=4, a(6)=4,
 * a(7)=2, a(8)=2, a(9)=2,
 * a(10)=3, a(11)=3, a(12)=3
 * The sequence is computed for all n ≥ 0 by a linear recurrence.
 */

#include <stddef.h>

extern seq_t a(idx_t n);

static const seq_t init[13] = {
    1, 1, 1, 1, 4, 4, 4, 2, 2, 2, 3, 3, 3
};

seq_t a(idx_t n) {
    if (n < 13) {
        return init[(size_t)n];
    }

    /* Ring buffer of the last 13 values. */
    seq_t buf[13];
    for (size_t i = 0; i < 13; ++i) {
        buf[i] = init[i];
    }

    for (idx_t i = 13; i <= n; ++i) {
        idx_t i1  = (i - 1) % 13;
        idx_t i12 = (i - 12) % 13;
        idx_t i13 = (i - 13) % 13;
        buf[i % 13] = buf[i1] + buf[i12] - buf[i13];
    }

    return buf[n % 13];
}
