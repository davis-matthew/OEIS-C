/*
 * A060136  Square array read by antidiagonals with
 *          T(n,k)=T(n,k-1)^2+n*T(n,k-1)+1 and T(n,0)=0.
 * https://oeis.org/A060136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 20, u32 n <= 27, u64 n <= 35, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A060136
   Sentinel function computing the nth term a(n) of the
   square array read by antidiagonals with the recurrence:
   T(n,0) = 0
   T(n,k) = T(n,k-1)^2 + n*T(n,k-1) + 1   (k>0)
   a(n) is obtained by traversing the array along antidiagonals;
   i.e. entry (r,k) with r + k = d is listed after all entries
   whose row+column sums are < d.  The mapping from a linear index
   n to coordinates (r,k) is: let d = floor((sqrt(1+8n)-1)/2),
   r = n - d*(d+1)/2, k = d - r.\n
   The code works for all n >= 0 and for seq_t of any unsigned
   integral type (uint8_t ... unsigned __int128, big integer class).
   It uses only integer arithmetic, no floating point or stdlib
   string/print facilities. */

#include <stdint.h>

/* Integer square root of an unsigned __int128 value */
static idx_t isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1) << 64;  /* 2^64 > sqrt(2^128-1) */
    unsigned __int128 result = 0;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 mid2 = mid * mid;
        if (mid2 <= x) {
            result = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return (idx_t)result;
}

/* Compute T(n,k) via the recurrence. */
static seq_t T_value(idx_t n, idx_t k)
{
    seq_t y = (seq_t)0;
    seq_t n_val = (seq_t)n;
    for (idx_t i = 1; i <= k; ++i) {
        y = y * y + n_val * y + (seq_t)1;
    }
    return y;
}

seq_t a(idx_t n)
{
    /* Find diagonal d: largest integer with d*(d+1)/2 <= n. */
    unsigned __int128 s = ((unsigned __int128)8) * n + (unsigned __int128)1;
    idx_t d = isqrt_u128(s);
    d = (d - (idx_t)1) / 2;

    /* Number of entries before this diagonal. */
    unsigned __int128 prev = ((unsigned __int128)d * (d + (idx_t)1)) / (unsigned __int128)2;

    /* Row and column indices for the nth entry. */
    idx_t r = n - (idx_t)prev;      /* row index (0 ≤ r ≤ d) */
    idx_t k = d - r;                /* column index (0 ≤ k ≤ d) */

    return T_value(r, k);
}
