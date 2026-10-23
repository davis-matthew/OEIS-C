/*
 * A106408  Triangle, read by rows, where T(1,1) = 1; T(2,1) = T(2,2) = 2; for
 *          n > 2, T(n,n) = T(n-1,n-1) + T(n-2,n-2); T(n+1,n) = 2 * T(n,n);
 *          for all other entries, T(n,k) = T(n-1,k) + T(n-2,k).
 * https://oeis.org/A106408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) with * T(r,k) = F(r‑k+2) * F(k+1) * where F
 *            is the ordinary Fibonacci sequence (F(0)=0, F(1)=1)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   fib_fast_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_fast_local.h"
static idx_t row_of_index(idx_t n)
{
    /* Binary search for the row containing the flattened index n
     * (1‑based).  Uses the fact that the first index of row r is
     * r(r‑1)/2 + 1 and the last is r(r+1)/2. */
    if (n == 0) return 0;               /* unused, protects against n=0 */

    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;                      /* find upper bound */

    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset */

    /* Find the row r and the column k (both 1‑based) corresponding
       to the flattened index n. */
    idx_t r = row_of_index(n);
    unsigned __int128 before = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)before;           /* column index */

    /* Fibonacci index calculations. */
    idx_t idx1 = r - k + 2;                /* n‑k+2  */
    idx_t idx2 = k + 1;                    /* k+1    */

    /* Compute F(idx1) and F(idx2) using fast doubling. */
    seq_t f1, dummy;
    fib_fast_local(idx1, &f1, &dummy);     /* f1 = F(idx1)   */
    seq_t f2;
    fib_fast_local(idx2, &f2, &dummy);     /* f2 = F(idx2)   */

    return f1 * f2;                        /* a(n) = T(r,k)  */
}
