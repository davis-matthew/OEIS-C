/*
 * A193978  Mirror of the triangle A193977.
 * https://oeis.org/A193978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((k+1)*r^2 + 5*(k+1)*r - k^3 - 4k^2 + k + 4) / 2 where r
 *            = floor((sqrt(1+8n) - 1) / 2) k = n - r*(r+1)/2 (index inside
 *            the r‑th row of the triangle) This produces the sequence
 *            2,5,6,9,14,12,14,24,27,20,20,36,45,44,30,27,50,66,72,65,42,35,66,90,...
 *            which is sequence A193978 (the flattened t...
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer.
   Returns floor(sqrt(x)). */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = (idx_t)1ULL << 32;   /* sqrt(2^64-1) < 2^32 */
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Find row r and column k for index n. */
    idx_t val   = 8ULL * n + 1ULL;               /* 1 + 8n  */
    idx_t m     = isqrt_u64(val);                /* floor(sqrt(1+8n)) */
    idx_t r     = (m - 1) / 2;                   /* row index */
    idx_t k     = n - r * (r + 1) / 2;           /* column index inside the row */

    /* Compute w(r,k) by the derived quadratic formula. */
    seq_t row  = (seq_t)r;
    seq_t col  = (seq_t)k;

    seq_t term = ((col + 1) * row * row
                  + 5 * (col + 1) * row
                  - col * col * col
                  - 4 * col * col
                  + col + 4);
    term /= 2;
    return term;
}
