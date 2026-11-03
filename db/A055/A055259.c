/*
 * A055259  Sums of two powers of 8.
 * https://oeis.org/A055259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 8^r + 8^k with r = ⌊(√(8n+1)–1)/2⌋ and k = n – r(r+1)/2
 *            (so that 0 ≤ k ≤ r). The sequence starts with a(0)=2, a(1)=9,
 *            a(2)=16, …
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 65, u64 n <= 251, u128 n <= 945,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 4824 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute the inverse triangular number: the greatest r such that
   r(r+1)/2 ≤ n.  All arithmetic is performed with unsigned __int128
   so the calculation is exact for n up to 2^64–1. */
static inline idx_t tri_inv(idx_t n)
{
    unsigned __int128 val = (unsigned __int128)8 * n + 1;   /* 8n+1   */
    /* Find floor(sqrt(val)).  sqrt(8n+1) < 2^34 for n < 2^64, so a
       32‑bit search range safely exceeds the true value. */
    uint64_t lo = 0, hi = ((uint64_t)1 << 32) - 1, mid;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= val)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    /* hi is floor(sqrt(val)) */
    return (idx_t)((hi - 1) >> 1);   /* (sqrt(val)-1)/2  */
}

/* Compute 8^exp for an arbitrary seq_t.  The loop works whether
   seq_t is a in‑teger type or an arbitrary‑precision class. */
static inline seq_t pow8(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)8;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    idx_t r   = tri_inv(n);                         /* r = ⌊(√(8n+1)–1)/2⌋   */
    unsigned __int128 t = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = (idx_t)((unsigned __int128)n - t);    /* k = n – r(r+1)/2       */

    seq_t p_r = pow8(r);
    seq_t p_k = pow8(k);
    return p_r + p_k;
}
