/*
 * A122765  Triangle read by rows: Let p(k, x) = x*p(k-1, x) - p(k-2, x). Then
 *          T(k,x) = dp(k,x)/dx.
 * https://oeis.org/A122765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the (n‑th) element in the triangle read by rows where
 *            the triangle entry T(i,j) is T(i,j) = j * (-1)^{C(i-j+1,2)} *
 *            binomial( floor((i+j)/2), j ). a(1) = 1; a(2) = -1; a(3) = 2;
 * Range:     i8 n <= 48, i16 n <= 218, i32 n <= 921, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any n >= 1.
   Uses only integer arithmetic and the provided binomial() routine. */

#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Find the row i such that i*(i+1)/2 >= n, using binary search. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t i = low;                       /* row number (1‑based) */

    /* Column j within that row: number of elements before this row + 1. */
    idx_t j = n - ((i - 1) * i) / 2;      /* column number (1‑based) */

    /* Sign factor (-1)^{C(i-j+1,2)}. */
    idx_t x = i - j + 1;
    __int128 comb = (__int128)x * (x - 1) / 2;   /* C(x,2) */
    seq_t sign = (comb & 1) ? -1 : 1;            /* -1 or +1 as seq_t */

    /* Binomial coefficient floor((i+j)/2) choose j. */
    idx_t y = (i + j) / 2;
    seq_t binom = binomial(y, j);

    /* Multiply by j and sign to get the triangle entry. */
    return binom * (seq_t)j * sign;
}
