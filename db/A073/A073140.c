/*
 * A073140  Sum of the largest and smallest number having in binary
 *          representation the same number of 0's and 1's as n.
 * https://oeis.org/A073140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A073137(n) + A073138(n) A073137(n) : smallest L‑bit
 *            number with exactly p ones, A073138(n) : largest L‑bit number
 *            with exactly p ones, where L = bit length of n (1 if n==0) and p
 *            = popcount(n)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   bit_length_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/bit_length_idx.h"

/* Count bits set in a 64‑bit integer. */
static inline idx_t popcount_u64(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    idx_t cnt = 0;
    while (x) { x &= x - 1; ++cnt; }
    return cnt;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t L = bit_length_idx(n);          /* number of bits in n   */
    idx_t p = popcount_u64(n);            /* number of ones in n   */

    /* Sudden positions are performed on big integer type seq_t,
       with the integer 1 promoted to at least 32 bits before shift. */
    seq_t msb = ((seq_t)1) << (size_t)(L - 1U);

    seq_t min_rest = ((seq_t)1) << (size_t)(p - 1U);
    min_rest -= (seq_t)1;
    seq_t min_val = msb + min_rest;

    seq_t max_high = ((seq_t)1) << (size_t)p;
    max_high -= (seq_t)1;
    seq_t max_val = max_high << (size_t)(L - p);

    return min_val + max_val;
}
