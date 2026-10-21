/*
 * A094954  Array T(k,n) read by antidiagonals. G.f.: x(1-x)/(1-kx+x^2), k>1.
 * https://oeis.org/A094954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   calc_T, sum_tri
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_T.h"
#include "../utils/sum_tri.h"
/* A094954: T(k,n) read by antidiagonals
 *
 *  T(k,1) = 1
 *  T(k,2) = k - 1
 *  T(k,m) = k*T(k,m-1) - T(k,m-2)   for m >= 3
 *
 *  For each n >= 1 let d be the smallest non‑negative integer with
 *  S(d) = (d+1)(d+2)/2 >= n.  Then
 *      offset = n - S(d-1) - 1   (0‑based position within the d‑th antidiagonal)
 *      k      = d + 2 - offset
 *      n2     = offset + 1
 *
 *  The required term is T(k,n2).
 *
 *  The implementation is O(d) which is O(√n).  All arithmetic on seq_t
 *  uses only integer operators, and all indices are of type idx_t.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n<1, but return 0 to be safe */

    /* Find minimal d with sum_tri(d) >= n   (binary search) */
    idx_t low = 0, high = 1;
    while (sum_tri(high) < n)
    {
        if (high > (UINT64_MAX >> 1)) {  /* guard against overflow */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (sum_tri(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;

    /* Previous sum S(d-1) */
    idx_t prev = (d == 0) ? 0 : sum_tri(d - 1);

    /* offset within the d-th antidiagonal (0‑based) */
    idx_t offset = n - prev - 1;

    /* k and n2 */
    idx_t k   = d + 2 - offset;   /* k >= 2 */
    idx_t n2  = offset + 1;       /* n2 >= 1 */

    return calc_T(k, n2);
}
