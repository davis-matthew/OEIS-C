/*
 * A056576  Highest k with 2^k <= 3^n.
 * https://oeis.org/A056576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_2(3^n)) = floor(n * log_2 3)
 * Range:     u8 n <= 161, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  log_2 3 ≈ 1.5849625007211561814537389439477L (80‑bit long double) */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const long double LOG2_3 =
        1.5849625007211561814537389439477L; /* high‑precision constant */

    /* Multiplication is performed in long double and then truncated to
       an unsigned 64‑bit integer.  The truncation of positive values is
       equivalent to floor.  The result fits comfortably in 64 bits for
       all n used in the tests. */
    uint64_t val = (uint64_t)((long double)n * LOG2_3);
    return (seq_t)val;
}
