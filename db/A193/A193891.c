/*
 * A193891  Triangular array: the self-fusion of (p(n,x)), where
 *          p(n,x)=x^n+2x^(n-1)+3x^(n-2)+...+nx+(n+1).
 * https://oeis.org/A193891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r = ⌊(√(1+8n) – 1)/2⌋ and k = n – r(r+1)/2. For r > 0 a(n) =
 *            ((k+1)(k+2)(3r – k))/6; a(0) = 1. (The array element (r,k) of
 *            the self‑fusion is given by that formula.)
 * Range:     u8 n <= 18, u16 n <= 1766, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of a 64‑bit unsigned integer, floor(sqrt(x)). */
static inline idx_t isqrt_u64(idx_t x)
{
    /* Binary search using 128‑bit multiplication to avoid overflow. */
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;        /* floor of sqrt(x) */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute r = floor((√(1+8n) – 1)/2). */
    idx_t t = isqrt_u64(1 + 8 * n);
    idx_t r = (t - 1) >> 1;      /* same as (t-1)/2 */

    /* k = position within row r. */
    idx_t base = r * (r + 1) / 2; /* number of terms before row r */
    idx_t k = n - base;           /* 0 ≤ k ≤ r */

    /* a(n) = ((k+1)(k+2)(3r – k))/6. */
    /* Perform all arithmetic in seq_t to avoid overflow. */
    seq_t k1 = (seq_t)(k + 1);
    seq_t k2 = (seq_t)(k + 2);
    seq_t r3k = (seq_t)(3 * r - k);
    seq_t prod = k1 * k2;
    prod = prod * r3k;
    return prod / 6;
}
