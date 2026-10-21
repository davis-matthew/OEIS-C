/*
 * A218447  a(n) = Sum_{k>=0} floor(n/(5*k + 4)).
 * https://oeis.org/A218447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k>=0} floor(n/(5*k + 4))
 * Range:     u8 n <= 277, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 4 no summands, result is 0. */
    if (n < 4) return 0;

    /* The largest k such that 5*k + 4 <= n. */
    idx_t last_k = (n - 4) / 5;

    seq_t sum = 0;
    for (idx_t k = 0; k <= last_k; ++k)
    {
        /* Compute the denominator safely: 5*k might overflow 64‑bit. */
        seq_t denom = ((seq_t)5 * (seq_t)k) + 4;
        sum += (seq_t)n / denom;
    }
    return sum;
}
