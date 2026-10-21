/*
 * A092283  Triangular array read by rows: T(n,k)=n+k^2, 1<=k<=n.
 * https://oeis.org/A092283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the (n‑th) term of the triangular array read by rows
 *            where T(r,c) = r + c^2 (1 ≤ c ≤ r). The array is indexed
 *            starting at 1: a(1) = T(1,1) = 2. For a given linear index i, we
 *            find the row r such that (r−1)·r/2 < i ≤ r·(r+1)/2 and the
 *            column c = i − (r−1)·r/2. Then a(i) = r + c^2. The impl...
 * Range:     u8 n <= 135, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Find the row r (1‑based) that contains the linear index idx in the
   triangular array.  Uses binary search with 128‑bit intermediate
   arithmetic to avoid overflow. */
static idx_t
row_of_index(idx_t idx)
{
    idx_t low = 1, high = 1;
    /* Grow high until we are assured that high*(high+1)/2 >= idx. */
    while (((unsigned __int128)high * (high + 1)) / 2 < idx)
        high <<= 1;
    /* Binary search for smallest r with r*(r+1)/2 >= idx. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t triangular = ( (__uint128_t)mid * (mid + 1) ) / 2;
        if (triangular >= idx)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    if (n == 0)  /* undefined by array, guard just in case */
        return (seq_t)0;
    /* Find row (r) and column (c) for linear index n. */
    idx_t r = row_of_index(n);
    idx_t prev_sum = ((unsigned __int128)(r - 1) * r) / 2;  /* ≤ n-1 */
    idx_t c = n - prev_sum;  /* 1-based column within the row */
    /* Compute a(n) = r + c^2 */
    seq_t rs = (seq_t)r;
    seq_t csq = (seq_t)c * (seq_t)c;
    return rs + csq;
}
