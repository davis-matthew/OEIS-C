/*
 * A047420  Numbers that are congruent to {0, 1, 2, 3, 4, 6} mod 8.
 * https://oeis.org/A047420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/6) + g[(n-1) mod 6] where g = {0,1,2,3,4,6}
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t pattern[6] = { (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)4, (seq_t)6 };

seq_t a(idx_t n)
{
    /* n >= 1 is assumed; behaviour for n < 1 is unspecified */
    idx_t k = (n - 1) / 6;            /* quotient of (n-1) by 6 */
    idx_t t = (n - 1) % 6;            /* remainder of (n-1) by 6 */
    seq_t base = (seq_t)8 * (seq_t)k; /* base term 8*k */
    return base + pattern[t];
}
