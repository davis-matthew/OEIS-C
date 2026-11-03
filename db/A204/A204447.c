/*
 * A204447  Symmetric matrix: f(i,j)=floor[(i+j+5)/4]-floor[(i+j+2)/4], by
 *          (constant) antidiagonals.
 * https://oeis.org/A204447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 99 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204447: values of the symmetric 0/1 matrix
 * with entries f(i,j) = floor[(i+j+5)/4] - floor[(i+j+2)/4]
 * read along successive antidiagonals.
 * a(n) is the value of the n-th entry when the matrix is
 * enumerated by antidiagonals.  Let t(n) be the smallest integer
 * t ≥ 2 with (t-1)*t/2 ≥ n.  Then
 *   a(n) = 0  if  t(n) mod 4 == 2
 *   a(n) = 1  otherwise
 */

#include <stdint.h>

/* Find the minimal t ≥ 2 such that (t-1)*t/2 ≥ n.
 * Uses binary search on t with 128‑bit arithmetic to
 * avoid overflow.                                  */
static inline idx_t _first_t_ge(idx_t n)
{
    idx_t low = 1, high = 1;

    /* Exponentially increase upper bound */
    while ((((__uint128_t)(high - 1) * high) / 2) < n) {
        high <<= 1;
        if (high == 0) {            /* overflow: fall back to max shift */
            high = (idx_t)1 << 63;
            break;
        }
    }

    /* Binary search between low and high inclusive */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sum = ((__uint128_t)(mid - 1) * mid) / 2;
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;                       /* this t satisfies (t-1)*t/2 ≥ n */
}

seq_t a(idx_t n)
{
    /* n is assumed to be ≥ 1 because the sequence offset is 1 */
    idx_t t = _first_t_ge(n);

    /* Antidiagonal sum s equals t.
       The matrix entry is 0 exactly when s mod 4 == 2. */
    return ((t & 3U) == 2) ? (seq_t)0 : (seq_t)1;
}
