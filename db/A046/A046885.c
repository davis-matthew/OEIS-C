/*
 * A046885  Row sums of triangle A046658.
 * https://oeis.org/A046885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - Catalan(n-1), a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (sequence A046885) */
#include <stdint.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Sequence is defined for n ≥ 1; return 0 for safety */
        return 0;
    }

    seq_t result = 1;           /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cat = catalan(i - 1);   /* Catalan(n-1) */
        result = result * 5 - cat;    /* a(i) = 5*a(i-1) - C(i-1) */
    }
    return result;
}
