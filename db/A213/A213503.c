/*
 * A213503  Rectangular array: (row n) = b**c, where b(h) = h^2, c(h) = n-1+h,
 *          n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(row,col) with row,col obtained from
 *            falling‑antidiagonal * reading of the array whose (n,k) entry is
 *            * * T(n,k) = k * (k^3 + 4*k^2*n + 6*k*n - k + 2*n) / 12 * * The
 *            mapping from the global index n (starting at 1) to (row,col) is:
 *            * let d = minimal integer with (d+1)(d+2)/2 >= n. * let pr...
 * Range:     u8 n <= 21, u16 n <= 406, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#if defined(__SIZEOF_INT128__)
  typedef unsigned __int128 u128;
#else
  /* Fallback for compilers without 128‑bit integer support.
     In practice the sequence does not require >64‑bit intermediate
     for the ranges tested in the small builds. */
  typedef unsigned long long u128;
#endif

/* Compute T(row, col) using 128‑bit integer arithmetic for safety. */
static inline seq_t T(seq_t row, seq_t col)
{
    u128 k = (u128)col;
    u128 n = (u128)row;

    u128 k2 = k * k;
    u128 k3 = k2 * k;

    /* sum = k^3 + 4*k^2*n + 6*k*n - k + 2*n */
    u128 sum = k3 + 4 * k2 * n + 6 * k * n - k + 2 * n;

    u128 num = k * sum;
    u128 res = num / 12;          /* exact division by 12 */

    return (seq_t)res;
}

/* Find the minimal diagonal d such that (d+1)(d+2)/2 >= n.
   n is 1‑based and assumed >= 1. */
static inline idx_t find_diagonal(idx_t n)
{
    if (n <= 1) {
        return 0;
    }

    idx_t low  = 0;
    idx_t high = 1;

    /* Increase high until it covers n.  Use u128 to avoid overflow. */
    while ((u128)(high + 1) * (high + 2) / 2 < n) {
        high <<= 1;
    }

    /* Binary search for the smallest d satisfying the condition. */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((u128)(mid + 1) * (mid + 2) / 2 >= n) {
            high = mid;
        } else {
            low = mid;
        }
    }
    return high;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;        /* undefined for indices below offset */
    }

    /* Locate the diagonal containing the nth term. */
    idx_t d = find_diagonal(n);

    /* Number of terms before this diagonal: prev = d*(d+1)/2. */
    u128 prev_cum = (u128)d * (d + 1) / 2;
    idx_t offset  = (idx_t)(n - (idx_t)prev_cum - 1);   /* 0‑based within the diagonal */

    idx_t row = offset + 1;          /* row number (n in T) */
    idx_t col = d - offset + 1;      /* column number (k in T) */

    return T((seq_t)row, (seq_t)col);
}
