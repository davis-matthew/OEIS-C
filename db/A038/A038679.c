/*
 * A038679  Convolution of A007054 (Super ballot numbers) with A000302 (powers
 *          of 4).
 * https://oeis.org/A038679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^(n+1) - C(n+1) (Catalan number)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

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

seq_t a(idx_t n)
{
    /* Cast n+1 to idx_t for the helper functions. */
    idx_t k = n + 1;
    seq_t four_pow = pow4(k);      /* 4^(n+1)  */
    seq_t cat      = catalan(k);   /* Catalan(n+1)  */
    return four_pow - cat;         /* difference  */
}
