/*
 * A074502  a(n) = 1^n + 2^n + 6^n.
 * https://oeis.org/A074502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 2^n + 6^n (with a(0)=3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_int.h"   /* seq_t pow_int(seq_t base, idx_t exp); */

seq_t a(idx_t n)
{
    /* 1^n is always 1 */
    seq_t pow2 = pow_int((seq_t)2, n);
    seq_t pow6 = pow_int((seq_t)6, n);
    return pow2 + pow6 + (seq_t)1;
}
