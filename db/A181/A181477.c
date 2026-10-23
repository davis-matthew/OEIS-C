/*
 * A181477  a(n) has generating function 1/((1-x)^k*(1-x^2)^(k*(k-1)/2)) for
 *          k=5.
 * https://oeis.org/A181477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{t=0}^{⌊n/2⌋} binom(9 + t, t) * binom(4 + n - 2*t, n -
 *            2*t)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 36, u64 n <= 223,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the provided binom function and works for any */
/* integer type seq_t supplied by the build (integer, unsigned __int128 or */
/* arbitrary precision). The function is independent of call order. */

#include <stddef.h>          /* for size_t */
#include "../utils/binom.h"  /* provides seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* Sum over t = 0 .. n/2 of binom(9 + t, t) * binom(4 + n - 2*t, n - 2*t) */
    seq_t sum = 0;
    for (idx_t t = 0; t <= n / 2; ++t) {
        /* small intermediate values (indices) fit in idx_t */
        seq_t part1 = binom((idx_t)(9 + t), (idx_t)t);
        seq_t part2 = binom((idx_t)(4 + n - 2 * t), (idx_t)(n - 2 * t));
        sum += part1 * part2;
    }
    return sum;
}
