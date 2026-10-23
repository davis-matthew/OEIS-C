/*
 * A123275  Square array A(n,m) = largest divisor of m which is coprime to n,
 *          read by upwards antidiagonals.
 * https://oeis.org/A123275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9681, u16 n <= 9681, u32 n <= 9681, u64 n <= 9681,
 *            u128 n <= 9681, big any n
 * Helpers:   largest_coprime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A123275: a(n) = largest divisor of m that is coprime to n, 
   read by upward antidiagonals of the square array A(i,j) where 
   A(i,j) = max{d | j : gcd(d,i)=1} */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/largest_coprime_divisor.h"

/* compute the triangular number T(x) = x*(x+1)/2 using 128-bit to avoid overflow */
static inline idx_t triangular(idx_t x)
{
    return (idx_t)(((__uint128_t)x * (x + 1)) >> 1);
}

/* find the smallest d such that T(d) >= n   (d is the diagonal index) */
static idx_t diagonal_of(idx_t n)
{
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (triangular(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* find diagonal d and offset along that diagonal */
    idx_t d = diagonal_of(n);
    idx_t prev = triangular(d - 1);
    idx_t offset = n - prev - 1;      /* zero‑based position on the diagonal */

    idx_t i = d - offset;            /* row index */
    idx_t j = offset + 1;            /* column index */

    return largest_coprime_divisor((seq_t)i, (seq_t)j);
}
