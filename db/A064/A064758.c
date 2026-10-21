/*
 * A064758  a(n) = n*12^n - 1.
 * https://oeis.org/A064758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*12^n - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   exp12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/exp12.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 as per the OEIS offset. */
    seq_t pow_val = exp12(n);          /* 12^n        */
    seq_t res     = (seq_t)n * pow_val;/* n * 12^n    */
    res -= 1;                          /* minus 1     */
    return res;
}
