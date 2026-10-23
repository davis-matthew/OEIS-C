/*
 * A047912  a(1) = 3; for n > 0, a(n+1) = a(n) * sum of digits of a(n).
 * https://oeis.org/A047912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, base
 * Formula:   a(1) = 3; for n > 0, a(n+1) = a(n) * digit_sum(a(n))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1 – guard for out‑of‑range */
        return (seq_t)0;

    seq_t x = (seq_t)3;         /* a(1) */

    for (idx_t i = 1; i < n; ++i) {
        seq_t s = digit_sum(x); /* sum of digits of a(i) */
        x = x * s;              /* a(i+1) */
    }

    return x;                   /* a(n) */
}
