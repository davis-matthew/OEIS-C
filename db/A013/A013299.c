/*
 * A013299  Expansion of e.g.f. -sinh(log(x+1) - arctanh(x)) (even powers
 *          only).
 * https://oeis.org/A013299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = a(n-1) * (n+1) * (2n+1) * (2n-1) / n for n >= 1
 *            (All operations are performed in seq_t, which may be any
 *            unsigned integer type.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t t1 = (seq_t)(i + 1);       /* (n+1) */
        seq_t t2 = (seq_t)(2 * i + 1);   /* (2n+1) */
        seq_t t3 = (seq_t)(2 * i - 1);   /* (2n-1) */
        res = res * t1;
        res = res * t2;
        res = res * t3;
        res = res / (seq_t)i;
    }
    return res;
}
