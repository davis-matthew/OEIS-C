/*
 * A020647  Least positive integer k for which 8^n divides k!.
 * https://oeis.org/A020647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that 8^n | k! (i.e. v₂(k!) ≥ 3·n)
 * Range:     u8 n <= 82, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                         /* 8^0 = 1 divides every factorial */
        return 1;

    idx_t k = 1;
    seq_t target = 3 * (seq_t)n;        /* 3·n */

    while (1) {
        /* compute exponent of 2 in k! */
        seq_t cnt = 0;
        for (idx_t p = 2; p <= k; p <<= 1)
            cnt += k / p;

        if (cnt >= target)
            return (seq_t)k;

        ++k;
    }
}
