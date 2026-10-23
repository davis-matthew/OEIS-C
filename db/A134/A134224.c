/*
 * A134224  A004736 + A134082 - I as infinite lower triangular matrices; I =
 *          Identity matrix.
 * https://oeis.org/A134224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   entries of the infinite lower triangular matrix A004736 +
 *            A134082 - I, indexed in row‑major order. For row r (0‑based) and
 *            column j (0‑≤ j ≤ r): a(r,r) = 1 a(r,r-1) = 2*(r+1) otherwise
 *            a(r,j) = (r - j + 1)
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 64‑bit number, using 128‑bit arithmetic to avoid overflow */
static inline idx_t isqrt_u64 (idx_t x)
{
    idx_t lo = 0, hi = 0xFFFFFFFFULL;
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 prod = (unsigned __int128)mid * mid;
        if (prod <= (unsigned __int128)x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* compute r*(r+1)/2 safely without overflow (using 128-bit) */
static inline idx_t tri (idx_t r)
{
    return (idx_t)(((unsigned __int128)r * (r + 1)) >> 1);
}

seq_t a (idx_t n)
{
    /* find the row index r such that tri(r) <= n < tri(r+1) */
    idx_t m = 8 * n + 1;
    idx_t sqrt_val = isqrt_u64 (m);      /* ⌊√(8n+1)⌋  */
    idx_t r = (sqrt_val - 1) >> 1;        /* ⌊(√(8n+1)-1)/2⌋  */
    
    idx_t tri_r = tri (r);
    idx_t j = n - tri_r;                  /* column within the row */
    
    if (j == r)                 /* diagonal */
        return (seq_t)1;
    else if (j == r - 1)        /* just before the diagonal */
        return (seq_t)2 * (seq_t)(r + 1);
    else                        /* all remaining entries */
        return (seq_t)(r - j + 1);
}
