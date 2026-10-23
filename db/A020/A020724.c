/*
 * A020724  G.f.: 1/((1-6*x)*(1-9*x)*(1-12*x)).
 * https://oeis.org/A020724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*12^n - 9*9^n + 2*6^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t p12 = (seq_t)1;
    seq_t p9  = (seq_t)1;
    seq_t p6  = (seq_t)1;

    for (idx_t i = (idx_t)0; i < n; ++i) {
        p12 = p12 * (seq_t)12;
        p9  = p9  * (seq_t)9;
        p6  = p6  * (seq_t)6;
    }

    seq_t term1 = p12 * (seq_t)8;
    seq_t term2 = p9  * (seq_t)9;
    seq_t term3 = p6  * (seq_t)2;

    return term1 - term2 + term3;
}
