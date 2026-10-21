/*
 * A066991  Square array read by descending antidiagonals of number of ways of
 *          dividing n*k labeled items into k unlabeled orders with n items in
 *          each order.
 * https://oeis.org/A066991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from the square array T(i,j) = (i*j)! / j! where
 *            the array is traversed by decreasing antidiagonals: first (1,1),
 *            then (1,2),(2,1), then (1,3),(2,2),(3,1), etc. For index n,
 *            determine the diagonal sum s = i + j and positions i (row) and j
 *            (column) as described below. The value is T(i...
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 23, u64 n <= 39, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root (floor) of a 64‑bit unsigned integer. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    idx_t bit = (idx_t)1 << 62;                /* start with highest power of four */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find diagonal sum s such that (s-1)*s/2 >= n. */
    idx_t val = 1 + 8 * n;                     /* 1 + 8*n */
    idx_t t   = isqrt_u64(val);                /* floor(sqrt(val)) */
    idx_t s   = (1 + t) / 2;
    /* Adjust s up if needed */
    while ((s - 1) * s / 2 < n) ++s;

    /* Position inside the diagonal */
    idx_t prev = (s > 2) ? (s - 2) * (s - 1) / 2 : 0;
    idx_t diag_start = prev + 1;
    idx_t offset     = n - diag_start;         /* zero‑based offset in diagonal */
    idx_t i = offset + 1;                      /* row index (n in product) */
    idx_t k = s - i;                           /* column index (k in product) */

    /* Compute T(i,k) = (i*k)! / k! = ∏_{t=k+1}^{i*k} t */
    seq_t result = 1;
    for (idx_t v = k + 1; v <= i * k; ++v) {
        result *= (seq_t)v;
    }
    return result;
}
