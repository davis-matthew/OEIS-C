/*
 * A128198  Array read by antidiagonals. A scheme of arrangements:
 *          ArrScheme(k,n) = VarScheme(k,n-1) + k^n; ArrScheme(k,0) = 1.
 *          VarScheme(k,n) = (n*k+1)*(VarScheme(k,n-1) + k^n); VarScheme(k,0)
 *          = 1.
 * https://oeis.org/A128198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = ArrScheme(k,j) where array indices (k,j) are obtained by
 *            reading an infinite 2‑D array row‑wise along its antidiagonals.
 *            ArrScheme(k,0) = 1 ; ArrScheme(k,n) = VarScheme(k,n-1) + k^n ;
 *            VarScheme(k,0) = 1 ; VarScheme(k,n) =
 *            (n*k+1)*(VarScheme(k,n-1)+k^n);
 * Range:     u8 n <= 24, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   arr_with_k_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/arr_with_k_n.h"

/* Find d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2  using binary search. */
static inline idx_t diag_of(idx_t n)
{
    idx_t lo = 0, hi = n;   /* hi is safe because d <= n for all n */
    idx_t ans = 0;

    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;

        if (tri <= n)
        {
            ans = mid;
            lo  = mid + 1;
        }
        else
            hi  = mid - 1;
    }
    return ans;
}

/* Public function requested by the build system. */
extern seq_t a(idx_t n)
{
    /* Locate the antidiagonal that contains the linear index n. */
    idx_t d = diag_of(n);

    /* Number of elements preceding diagonal d. */
    __int128 terms_before128 = (__int128)d * (d + 1) / 2;
    idx_t    terms_before = (idx_t)terms_before128;

    /* Offset inside the diagonal. */
    idx_t offset = n - terms_before;

    /* Map linear index to 2‑D array coordinates (k, j). */
    idx_t k = d - offset;
    idx_t j = offset;

    return arr_with_k_n(k, j);
}
