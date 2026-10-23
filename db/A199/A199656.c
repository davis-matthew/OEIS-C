/*
 * A199656  Triangular array read by rows, T(n,k) is the number of functions
 *          from {1,2,...,n} into {1,2,...,n} with maximum value of k.
 * https://oeis.org/A199656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = k^r - (k-1)^r where r = smallest integer with r(r+1)/2 ≥
 *            n, k = n - (r-1)r/2
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 54, u64 n <= 149,
 *            u128 n <= 402, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/big_pow.h"

/* Helper: compute the row r for index n */
static inline idx_t _triangle_row(idx_t n)
{
    if (n < 1) return 0;
    double d = sqrt((double)(8 * n + 1));
    idx_t r = (idx_t)((d - 1.0) / 2.0);      /* floor of the solution */
    while ((r * (r + 1)) / 2 < n) ++r;       /* adjust for rounding */
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined, return 0 */

    /* Find row r and column k */
    idx_t r = _triangle_row(n);
    idx_t offset_prev = ((r - 1) * r) / 2;
    idx_t k = n - offset_prev;               /* 1 ≤ k ≤ r */

    /* Compute k^r and (k-1)^r */
    seq_t pow_k    = big_pow((seq_t)k,    (seq_t)r);
    seq_t pow_km1  = big_pow((seq_t)(k - 1), (seq_t)r);

    return pow_k - pow_km1;
}
