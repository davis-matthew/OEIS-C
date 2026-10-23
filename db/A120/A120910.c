/*
 * A120910  Triangle read by rows: T(n,k) is the number of ternary words of
 *          length n having k levels (n >= 1, 0 <= k <= n-1). A level is a
 *          pair of identical consecutive letters.
 * https://oeis.org/A120910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(n,k) = 3*2^(n-k-1)*binomial(n-1,k) and r,k
 *            are the row, column indices obtained from the linear index n
 *            (row r has r elements, rows are 1‑based)
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n<1, return 0 safe */

    /* Find smallest r such that r*(r+1)/2 >= n (row number). */
    idx_t low  = 1;
    idx_t high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= n) high = mid;
        else          low  = mid + 1;
    }
    idx_t r = low;   /* row index */

    /* Compute start of this row: first linear index in row r. */
    unsigned __int128 start = (unsigned __int128)r * (r - 1) / 2 + 1;
    idx_t k = n - (idx_t)start;      /* column index (0‑based) */

    /* Compute T(r,k) = 3 * 2^(r-k-1) * binomial(r-1,k). */
    seq_t val = (seq_t)3;
    val *= pow2(r - k - 1);
    val *= binomial(r - 1, k);
    return val;
}
