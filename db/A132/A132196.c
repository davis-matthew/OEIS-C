/*
 * A132196  a(2n+1) = 2a(2n)-n for the odd indices. Smallest strictly positive
 *          integer not yet in the list for the even indices, a(2n).
 * https://oeis.org/A132196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (m + n)/2 for even n * a(n) = m + (n-1)/2 for odd n *
 *            where m is the greatest power of 2 not exceeding n+2. * * This
 *            implements sequence A132196 (offset 0)
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_log2_u64(idx_t v)
{
    /* v > 0 for all invocations (n+2 ≥ 1) */
    idx_t r = 0;
    while (v >>= 1)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Compute m = 2^⌊log2(n+2)⌋ */
    idx_t v = n + 2;
    idx_t h = floor_log2_u64(v);
    idx_t m = (idx_t)1 << h;  /* greatest power of 2 ≤ v */

    if ((n & (idx_t)1) == 0)  /* even index */
        return ((seq_t)m + (seq_t)n) / (seq_t)2;
    else  /* odd index */
        return (seq_t)m + ((seq_t)(n - 1) / (seq_t)2);
}
