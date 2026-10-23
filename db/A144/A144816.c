/*
 * A144816  Denominators of triangle T(n,k), n>=0, 0<=k<=n, read by rows:
 *          T(n,k) is the coefficient of x^(2*k+1) in polynomial t_n(x), used
 *          to define continuous and n times differentiable sigmoidal transfer
 *          functions.
 * https://oeis.org/A144816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 152, u64 n <= 560,
 *            u128 n <= 2144, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A144816 – Denominators of triangle T(n,k)
 *
 * For a given index n, locate the row r and column k:
 *   r = floor( (sqrt(8*n+1)-1)/2 )
 *   k = n - r*(r+1)/2
 *
 * Then
 *   e = 2*r - popcount(r) - ( popcount(k) + popcount(r-k) - popcount(r) )
 *   a(n) = 2^e
 *
 * Formula (OEIS notation):
 *   a(n) = 2^{ 2*r - pc(r) - ( pc(k) + pc(r-k) - pc(r) ) }
 *   where r = ⌊(√(8n+1)-1)/2⌋, k = n - r(r+1)/2, and pc(x) is the number
 *   of 1-bits in binary representation of x (popcount).
 *
 * The sequence starts:
 *   1, 2, 2, 8, 4, 8, 16, 16, 16, 16, 128, 32, 64, 32, 128, 256, …
 *
 * No hard‑coded table is used; the values are computed directly from
 * the above formula.  The implementation uses only integer arithmetic
 * and the helper seq_pow from the utilities for arbitrary‑precision
 * arithmetic when needed.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/seq_pow.h"

/* popcount for 64‑bit values --------------------------------------- */
static inline unsigned int popcnt64(uint64_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    unsigned int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
#endif
}

/*------------------------------------------------------------------------
 *  Compute the nth term of A144816
 *-----------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* 1. Determine the row r.  Use double sqrt; correct possible rounding. */
    uint64_t r = (uint64_t)((sqrt((double)(8ULL * (unsigned long long)n + 1ULL)) - 1.0) / 2.0);
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n)   --r;

    /* 2. Column index within the row. */
    uint64_t k = n - r * (r + 1) / 2;

    /* 3. Popcount values. */
    unsigned int pc_r  = popcnt64(r);
    unsigned int pc_k  = popcnt64(k);
    unsigned int pc_rk = popcnt64(r - k);

    /* 4. Exponent e = 2*r - pc(r) - (pc(k)+pc(r-k)-pc(r)) */
    int64_t   base  = (int64_t)2 * (int64_t)r - (int64_t)pc_r;
    int64_t   v2    = (int64_t)pc_k + (int64_t)pc_rk - (int64_t)pc_r;
    int64_t   e_int = base - v2;          /* e is always non‑negative */

    idx_t e = (idx_t)e_int;

    /* 5. Return 2^e.  Use seq_pow for arbitrary‑precision types. */
    return seq_pow((seq_t)2, e);
}
