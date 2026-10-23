/*
 * A008822  Expansion of (1 + 2*x^2 + x^3)/((1 - x)^2*(1 - x^3)).
 * https://oeis.org/A008822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n+1)*(2*n+1)/3)
 * Range:     u8 n <= 18, u16 n <= 312, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use 64‑bit arithmetic to avoid intermediate overflow for small seq_t */
    uint64_t n64 = (uint64_t)n;
    uint64_t n1   = n64 + 1;
    uint64_t n2   = n64 * 2 + 1;
    uint64_t prod = n1 * n2;
    uint64_t res  = (prod + 2) / 3;   /* ceil division by 3 */
    return (seq_t)res;
}
