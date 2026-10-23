/*
 * A032514  Sum of the integer part of 3/2-th roots of integers less than n.
 * https://oeis.org/A032514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} ⌊k^(2/3)⌋
 * Range:     u8 n <= 14, u16 n <= 643, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The term ⌊k^(2/3)⌋ is computed as floor(cuberoot(k*k)),
   using a binary‑search integer cube root. */

#include <stdint.h>   /* for uint64_t if needed */

static seq_t int_cuberoot(seq_t x)
{
    if (x == 0) return 0;
    seq_t low = 0, high = 1;
    /* Find an upper bound high such that high^3 > x */
    while ((high * high * high) <= x) high = high * (seq_t)2;
    /* Binary search between low and high */
    while ((low + (seq_t)1) < high) {
        seq_t mid = (low + high) / 2;
        seq_t mid3 = mid * mid * mid;
        if (mid3 <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k * (seq_t)k;          /* k^2 */
        seq_t r  = int_cuberoot(kk);             /* floor(k^(2/3)) */
        sum += r;
    }
    return sum;
}
