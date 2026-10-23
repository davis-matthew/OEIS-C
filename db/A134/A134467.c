/*
 * A134467  a(n) = n(n+1) - A000120(n), where A000120(n) = number of 1's in
 *          binary expansion of n.
 * https://oeis.org/A134467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1) - popcount(n)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* compute n*(n+1) in seq_t arithmetic */
    seq_t prod = nn * (nn + 1);
    /* popcount for 64‑bit idx_t */
    unsigned int pop = __builtin_popcountll(n);
    return prod - (seq_t)pop;
}
