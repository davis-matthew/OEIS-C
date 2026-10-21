/*
 * A070468  a(n) = n^2 mod 46.
 * https://oeis.org/A070468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 46
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t mod = n % 46;               /* reduce n modulo 46 */
    seq_t r = (seq_t)mod;            /* r ∈ [0,45] */
    uint64_t sq = (uint64_t)r * (uint64_t)r; /* safe intermediate */
    return (seq_t)(sq % 46);
}
