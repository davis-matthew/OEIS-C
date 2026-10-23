/*
 * A135764  Distribute the natural numbers in columns based on the occurrence
 *          of "2" in each prime factorization; square array A(row,col) =
 *          2^(row-1) * ((2*col)-1), read by descending antidiagonals.
 * https://oeis.org/A135764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = A135764 sequence element. The sequence is obtained by
 *            reading the infinite array A(row,col) = 2^(row-1) * (2*col-1)
 *            along descending antidiagonals
 * Range:     u8 n <= 42, u16 n <= 150, u32 n <= 558, u64 n <= 2142,
 *            u128 n <= 8382, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* Integer square root of an unsigned __int128 x, returning the largest r
   such that r*r <= x. */
static inline idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t low = 0, high = (idx_t)1 << 34;          /* 2^34 > sqrt(2^67) */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* offset is 1; value undefined for n==0 */

    /* Find which diagonal (s = k+1) contains the n-th element. */
    unsigned __int128 v = ((unsigned __int128)n << 3) + 1;   /* 8*n + 1 */
    idx_t sqrt_v = isqrt_u128(v);
    idx_t k = (sqrt_v - 1) >> 1;                    /* floor((sqrt-1)/2) */

    /* Ensure k is the smallest integer with T_k >= n, where T_k = k*(k+1)/2. */
    unsigned __int128 T_k128 = (__int128)k * (k + 1) >> 1;
    idx_t T_k = (idx_t)T_k128;
    if (T_k < n) ++k;

    /* Compute offset within the diagonal. */
    unsigned __int128 T_prev128 = (__int128)(k - 1) * k >> 1;
    idx_t T_prev = (idx_t)T_prev128;
    idx_t offset = n - T_prev - 1;          /* 0‑based offset in the diagonal */

    idx_t row = offset + 1;                 /* 1‑based row index */
    idx_t col = k - offset;                 /* 1‑based column index */

    seq_t power = pow2(row - 1);            /* 2^(row-1) */
    seq_t odd   = (seq_t)(2 * col - 1);     /* (2*col-1) */

    return power * odd;
}
