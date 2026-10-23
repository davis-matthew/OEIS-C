/*
 * A048967  Number of even entries in row n of Pascal's triangle (A007318).
 * https://oeis.org/A048967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + 1 - 2^{popcount(n)} (number of even entries in row n
 *            of Pascal's triangle)
 * Range:     u8 n <= 259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the number of 1‑bits in a 64‑bit unsigned integer. */
static inline idx_t popcount_u64(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    idx_t cnt = 0;
    while (x) {
        x &= x - 1;      /* turn off the lowest set bit */
        ++cnt;
    }
    return cnt;
#endif
}

extern seq_t a(idx_t n)
{
    /* 2^{popcount(n)} */
    idx_t pc   = popcount_u64(n);          /* popcount of n */
    seq_t odd  = ((seq_t)1) << pc;         /* 2^popcount(n) */

    /* total entries in the row */
    seq_t total = ((seq_t)n) + 1;

    return total - odd;
}
