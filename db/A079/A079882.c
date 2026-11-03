/*
 * A079882  A run of 2^n 1's followed by a run of 2^n 2's, for n=0, 1, 2, ...
 * https://oeis.org/A079882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if the position falls in the 1‑run of its block,
 *            otherwise 2. The block containing index n has length 2^(k+1)
 *            where k = floor(log2(n+2)) – 1, i.e. its start is 2^(k+1) – 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9604 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Author: ChatGPT – simple logarithmic computation */

#include <stdint.h>

static inline seq_t A079882(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow of 2^64 lengths. */
    __uint128_t start = 0;      /* start index of the current block   */
    __uint128_t len   = 2;      /* block length = 2^(k+1), start at 2 */

    /* Find the block containing n. */
    while ((__uint128_t)n >= start + len) {
        start += len;
        len <<= 1;              /* double the block length for next k   */
    }

    __uint128_t offset = (__uint128_t)n - start;   /* position inside block */
    /* The first half of the block consists of 1's. */
    if (offset < (len >> 1))
        return (seq_t)1;
    else
        return (seq_t)2;
}

seq_t a(idx_t n) { return A079882(n); }
