/*
 * A216228  Square array T, read by antidiagonals: T(n,k) = 0 if n-k>=1 or if
 *          k-n>=3, T(0,0) = T(0,1) = T(0,2) = 1, T(n,k) = T(n-1,k) +
 *          T(n,k-1).
 * https://oeis.org/A216228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the infinite 2‑D array T by
 *            antidiagonals. T(i,j)=0 if i>j or j>i+2. Otherwise: T(0,0)=1,
 *            T(0,1)=T(0,2)=1. For j==i (i>0): T(i,i)=2^{i-1}. For j==i+1 or
 *            j==i+2: T(i,j)=2^{i}
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of zeros and powers of two. */

#include <math.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Determine the diagonal s such that the first s+1 diagonals contain at least n+1 elements. */
    double d = sqrt((double)8 * n + 1);
    idx_t s = (idx_t)((d - 1) / 2);  /* initial estimate */

    /* Correct possible round‑off errors. */
    while ((unsigned __int128)(s + 1) * (s + 2) / 2 <= n) s++;
    while (s > 0 && (unsigned __int128)s * (s + 1) / 2 > n) s--;

    unsigned __int128 prev = (unsigned __int128)s * (s + 1) / 2;
    idx_t offset = n - (idx_t)prev;          /* position within diagonal s */
    idx_t i = offset;                        /* row index */
    idx_t j = s - offset;                    /* column index */

    /* Compute T(i,j) directly. */
    if (i > j) return (seq_t)0;
    if (j > i + 2) return (seq_t)0;
    if (j == i) {                /* diagonal element */
        if (i == 0) return (seq_t)1;
        return pow2(i - 1);
    }
    /* j == i+1 or j == i+2 */
    return pow2(i);
}
