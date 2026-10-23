/*
 * A178831  Rectangular array T(n,k) = binomial(n+1,2)*(n^k - (n-1)^k) read by
 *          antidiagonals.
 * https://oeis.org/A178831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the nth element of the array
 *            T(n,k)=binomial(n+1,2)*(n^k-(n-1)^k) read by antidiagonals
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 125, u64 n <= 358,
 *            u128 n <= 1001, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* not defined for 0, but safe guard */

    /* find s such that s*(s-1)/2 >= n */
    idx_t low = 2;
    idx_t high = n + 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid - 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;                      /* sum index (n + k) */

    /* previous cumulative count */
    idx_t prev = (idx_t)((__int128)(s - 1) * (s - 2) / 2);

    /* position within the diagonal */
    idx_t p   = n - prev - 1;            /* 0‑based */
    idx_t row = p + 1;                   /* n in T(n,k) */
    idx_t col = s - row;                 /* k in T(n,k) */

    /* compute binomial(row+1,2) * (row^col - (row-1)^col) */
    seq_t binom = ((seq_t)row * (row + 1)) / 2;
    seq_t pow_row = big_pow((seq_t)row, (seq_t)col);
    seq_t pow_prev = big_pow((seq_t)(row - 1), (seq_t)col);
    seq_t diff = pow_row - pow_prev;

    return binom * diff;
}
