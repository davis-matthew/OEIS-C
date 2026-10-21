/*
 * A059161  A hierarchical sequence (W'2{3}*c - see A059126).
 * https://oeis.org/A059161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12 * 2^(v_2(n+1)) (v_2 is the 2‑adic valuation)
 * Range:     u8 n <= 30, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of OEIS sequence A059161.
   The value is 12 times the lowest power of two dividing n+1.
*/
seq_t a(idx_t n)
{
    /* Convert to 64‑bit integer for the low‑bit calculation. */
    uint64_t x = (uint64_t)n + 1ULL;         /* n+1 */
    uint64_t low = x & (uint64_t)(-x);       /* lowest set bit of n+1 (2^v2(n+1)) */

    /* Use 128‑bit multiplication to avoid overflow when seq_t is 64 bits or larger. */
    return (seq_t)((unsigned __int128)12 * low);
}
