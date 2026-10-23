/*
 * A124848  Triangle read by rows: T(n,k) = (k+1)*(k+2)*(k+3)*binomial(n,k)/6
 *          (0 <= k <= n).
 * https://oeis.org/A124848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, and T(r,k) = (k+1)(k+2)(k+3)/6 * binomial(r,k)
 * Range:     u8 n <= 19, u16 n <= 83, u32 n <= 362, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Find the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* Exponentially increase high until it exceeds the target */
    while ((((unsigned __int128)high * (high + 1)) / 2) <= n) {
        high <<= 1;
    }
    /* Binary search for the largest r with triangular <= n */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;   /* mid > low */
        unsigned __int128 tri = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);                 /* row index */
    idx_t offset = (r * (r + 1)) / 2;      /* number of terms before row r */
    idx_t k = n - offset;                  /* column within the row */

    seq_t bin = binomial(r, k);            /* binomial coefficient */

    /* Compute (k+1)(k+2)(k+3)/6 safely */
    seq_t coeff = (seq_t)(k + 1);
    coeff *= (k + 2);
    coeff *= (k + 3);
    coeff /= 6;

    return coeff * bin;
}
