/*
 * A138771  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} whose 2nd cycle has k entries; each cycle is written
 *          with the smallest element first and cycles are arranged in
 *          increasing order of their first elements (n>=1; 0<=k<=n-1). For
 *          example, 1432=(1)(24)(3) has 2 entries in the...
 * https://oeis.org/A138771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = minimal s.t. s(s+1)/2 >= n, k = n -
 *            (r-1)r/2 - 1. Recurrence: T(1,0) = 1 T(n,0) = (n-1)! T(n,k) =
 *            (n-1) * T(n-1,k) + (n-2)! (1 <= k <= n-2) T(n,n-1) = (n-2)!
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Maximum row size needed for n up to about 10 000 (r ≈ 141).  
   A generous bound keeps the stack usage low and covers the allowed range. */
#define MAX_ROW 2000

/* Find the row r such that r*(r+1)/2 >= n. */
static idx_t
find_row(idx_t n)
{
    idx_t lo = 1, hi = 1;
    while (((unsigned __int128)hi * (hi + 1) / 2) < n) {
        hi <<= 1;
    }
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= n) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* out of range – harmless */

    idx_t r = find_row(n);
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t start = (idx_t)(prev_tri + 1);
    idx_t k = n - start;          /* 0 <= k <= r-1 */

    /* If r exceeds our static limit, the function is undefined. */
    if ((size_t)r > MAX_ROW)
        return 0;

    /* factorials up to r-1 */
    seq_t fact[MAX_ROW + 1];
    fact[0] = (seq_t)1;
    for (size_t i = 1; i <= r; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* buffers for consecutive rows */
    seq_t prev[MAX_ROW];
    seq_t curr[MAX_ROW];

    /* row 1 */
    prev[0] = fact[0];                /* T(1,0) = 1 */

    for (size_t row = 2; row <= r; ++row) {
        curr[0] = fact[row - 1];                  /* (row-1)! */

        /* k = 1 .. row-2 */
        for (size_t kk = 1; kk <= row - 2; ++kk) {
            curr[kk] = (seq_t)(row - 1) * prev[kk] + fact[row - 2];
        }

        /* last element k = row-1 */
        curr[row - 1] = fact[row - 2];

        /* copy current row to be the previous row for the next iteration */
        for (size_t kk = 0; kk < row; ++kk)
            prev[kk] = curr[kk];
    }

    return prev[(size_t)k];
}
