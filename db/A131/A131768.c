/*
 * A131768  2*(A007318 * A097807) - A000012.
 * https://oeis.org/A131768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2 * C(r+1 , j+1) – 1 where r = floor( (sqrt(8*n+1) – 1)/2
 *            ) (the row index) j = n – r*(r+1)/2 (position within the row,
 *            zero‑based)
 * Range:     u8 n <= 47, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) as a seq_t using the multiplicative
   formula (avoid factorial overflow).  Assumes n,k fit in idx_t. */
static seq_t binomial_coeff(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        unsigned __int128 tmp = (unsigned __int128)res * (n - i + 1);
        tmp /= i;
        res = (seq_t)tmp;
    }
    return res;
}

/* Find the largest r such that r*(r+1)/2 <= n.  Uses binary search on an
   exponentially increasing interval; safe for very large n. */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    /* exponential search for an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) high <<= 1;
    /* binary search between low and high */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* OEIS sequence A131768: a(n) = 2 * C(r+1, j+1) – 1 */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);                       /* row index */
    idx_t j = n - r * (r + 1) / 2;               /* position within the row */
    seq_t coeff = binomial_coeff(r + 1, j + 1);   /* C(r+1, j+1) */
    return coeff * (seq_t)2 - (seq_t)1;
}
