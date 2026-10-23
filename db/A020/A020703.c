/*
 * A020703  Take the sequence of natural numbers (A000027) and reverse
 *          successive subsequences of lengths 1,3,5,7,...
 * https://oeis.org/A020703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m² + (m‑1)² + 1 – n, where m = ceil(√n)
 * Range:     u8 n <= 225, u16 n <= 9361, u32 n <= 9361, u64 n <= 9361,
 *            u128 n <= 9361, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the nth term of OEIS A020703 (reverse successive odd‑length subsequences
 * of the natural numbers).  The result is computed in O(1) time, using only
 * integer arithmetic. */
#include <stdint.h>

/* Compute floor(sqrt(n)) for n up to 2⁶⁴−1.  No floating point or helper
 * functions are required. */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;          /* 0 or 1 */
    idx_t low = 0, high = ((idx_t)1 << 32); /* maximum possible sqrt */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        /* Use division to avoid overflow: mid*mid <= n  iff  mid <= n/mid */
        if (mid <= n / mid)
            low = mid;
        else
            high = mid;
    }
    return low;   /* floor sqrt */
}

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the caller.  Compute m = ceil(√n). */
    idx_t m = isqrt_u64(n);
    if (m * m < n) ++m;           /* adjust to ceiling */
    idx_t prev_sq = (m - 1) * (m - 1);   /* (m‑1)² */
    idx_t offset   = n - prev_sq - 1;   /* zero‑based position inside block */
    idx_t end = m * m;                     /* last number of the block */
    seq_t val = (seq_t)(end - offset);     /* reversed position */
    return val;
}
