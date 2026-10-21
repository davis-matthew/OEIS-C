/*
 * A059939  a(n) = floor(log_2(n+1) - 1).
 * https://oeis.org/A059939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(log_2(n+1)) - 1
 * Range:     u8 n <= 1024, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute floor(log2(n+1)) - 1.
       n >= 1, so (n+1) >= 2, and the result is non‑negative. */
    unsigned long long x = (unsigned long long)n + 1ULL;

    /* Find the position of the most significant 1‑bit in x.
       This is floor(log2(x)). */
    unsigned int msb = 0;
    unsigned long long tmp = x;
    while (tmp >>= 1) {
        ++msb;
    }

    /* msb is floor(log2(x)); subtract 1 to get a(n). */
    return (seq_t)(msb - 1);
}
