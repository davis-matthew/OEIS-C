/*
 * A204028  Symmetric matrix based on f(i,j)=min(3i-2,3j-2), by antidiagonals.
 * https://oeis.org/A204028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3*min(i, j) - 2 where (i, j) are the antidiagonal
 *            coordinates of n in the infinite symmetric matrix M with M[i][j]
 *            = min(3i-2, 3j-2) (i, j >= 1)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The enumeration follows the antidiagonal order:
   for each sum s = i + j (starting from s = 2),
   list positions (1, s-1), (2, s-2), … , (s-1, 1). */

#include <stdint.h>

static inline idx_t tri(idx_t k)
{
    /* Compute k*(k+1)/2 without overflow (use 128-bit) */
    return (idx_t)((unsigned __int128)k * (k + 1) / 2);
}

/* Find the smallest k such that k*(k+1)/2 >= n */
static idx_t find_k(idx_t n)
{
    idx_t low = 0, high = 1;
    while (tri(high) < n) high <<= 1;
    while (low < high)
    {
        idx_t mid = (low + high) >> 1;
        if (tri(mid) >= n) high = mid;
        else low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, but safe */

    idx_t k      = find_k(n);                     /* k = s - 1, where s is sum index */
    idx_t prev   = tri(k - 1);                    /* number of terms before sum s */
    idx_t offset = n - prev;                      /* 1‑based position on diagonal */
    idx_t i = offset;                            /* i coordinate (1 .. k) */
    idx_t j = (k + 1) - offset;                  /* j coordinate (k .. 1) */
    idx_t m = (i < j) ? i : j;                   /* min(i, j) */

    return (seq_t)3 * m - (seq_t)2;               /* M[i][j] = 3*min(i,j) - 2 */
}
