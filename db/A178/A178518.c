/*
 * A178518  Triangle read by rows: T(n,k) is the number of permutations p of
 *          {1,2,...,n} having genus 0 and such that p(1)=k (see first comment
 *          for definition of genus).
 * https://oeis.org/A178518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   read the triangle T(n,k) by rows; T(n,1)=c(n-1),
 *            T(n,k)=c(n-k+1)*c(k-2) for 2≤k≤n, where c(j)=Catalan(j)
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function a(n) returns the n-th term of the concatenated rows.   */
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, but harmless */

    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t tri = ((seq_t)mid * (mid + 1)) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t row = lo;

    /* Position within the row (1‑indexed) */
    seq_t prev_tri = ((seq_t)(row - 1) * row) / 2;
    idx_t pos = n - (idx_t)prev_tri;   /* pos ∈ [1, row] */

    if (pos == 1)
        return catalan(row - 1);

    seq_t left  = catalan(row - pos + 1);
    seq_t right = catalan(pos - 2);
    return left * right;
}
