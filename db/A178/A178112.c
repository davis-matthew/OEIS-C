/*
 * A178112  Number triangle T(n,k)=C(floor(n/2),floor(k/2))*(1+(-1)^(n-k))/2.
 * https://oeis.org/A178112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 260, u16 n <= 756, u32 n <= 2518, u64 n <= 9377,
 *            u128 n <= 9504, big any n
 * Helpers:   binom_nn
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 
 *   let r be the largest integer such that r*(r+1)/2 <= n,
 *   let k = n - r*(r+1)/2.
 *   a(n) = 0                      if (r - k) is odd,
 *   a(n) = C( floor(r/2), floor(k/2) ) otherwise.
 *
 * This implements the flat sequence A178112: the flattened triangle
 * T(r,k)=C(floor(r/2),floor(k/2))*((1+(-1)^(r-k))/2).
 */
#include <stdbool.h>   /* for bool, though unused */
#include "../utils/binom_nn.h"

/* Find the row index r for a given flattened index n */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = n, res = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri128 = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri128 <= n) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t r   = find_row(n);
    unsigned __int128 tri128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t tri = (idx_t)tri128;    /* r*(r+1)/2 <= n <= 2^64-1 fits in idx_t */
    idx_t k   = n - tri;

    /* Term is zero when (r-k) is odd */
    if (((r - k) & 1U) != 0U)
        return 0;

    idx_t ar = r >> 1;   /* floor(r/2) */
    idx_t ak = k >> 1;   /* floor(k/2) */

    if (ak > ar)            /* binomial undefined, term is zero */
        return 0;

    return binom_nn(ar, ak);
}
