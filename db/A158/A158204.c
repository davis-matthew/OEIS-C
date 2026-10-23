/*
 * A158204  Terms in A178335 not divisible by 10.
 * https://oeis.org/A158204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = 7 × the n‑th positive integer that is a power of 2 or 5,
   sorted in increasing order. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n < 1 is irrelevant */
    if (n == 0) return (seq_t)0;

    seq_t val2 = (seq_t)7;   /* 7·2^0 */
    seq_t val5 = (seq_t)7;   /* 7·5^0 */
    seq_t result = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i) {
        if (val2 < val5) {
            result = val2;
            val2 *= (seq_t)2;
        } else if (val5 < val2) {
            result = val5;
            val5 *= (seq_t)5;
        } else {                /* equal – skip duplicate */
            result = val2;
            val2 *= (seq_t)2;
            val5 *= (seq_t)5;
        }
    }
    return result;
}
