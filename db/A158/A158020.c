/*
 * A158020  Triangle of coefficients in expansion of
 *          (-1-x)^floor(n/2)(1-x)^ceiling(n/2).
 * https://oeis.org/A158020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the flattened triangle T(r,k) defined by T(r,k) =
 *            T(r-2,k-2) - T(r-2,k), with T(0,0)=T(1,0)=1, T(1,1)=-1. An
 *            equivalent explicit construction is: If r is even (r = 2m) then
 *            T(r,2i) = (-1)^(m-i) * C(m,i) (0 ≤ i ≤ m) T(r,2i+1) = 0 If r is
 *            odd (r = 2m+1) then T(r,2i) = (-1)^(m+i) * C(m,i) T(...
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* locate the row r that contains the n-th element of the
       flattened triangle.  r satisfies
          r*(r+1)/2   ≤  n  <  (r+1)*(r+2)/2    (0‑based indices) */
    idx_t r = 0;
    while (1) {
        idx_t tri = r * (r + 1) / 2;
        if (tri > n) {
            r--;          /* the previous r is the correct one          */
            break;
        }
        r++;              /* try the next row                           */
    }

    /* starting index of row r */
    idx_t start = r * (r + 1) / 2;
    /* column index within the row */
    idx_t k = n - start;

    /* compute the value according to the parity of r and k */
    if ((r & 1) == 0) {                /* r is even, r = 2m          */
        idx_t m = r >> 1;
        if ((k & 1) == 1) {            /* odd column → 0            */
            return 0;
        }
        idx_t i = k >> 1;              /* i = k/2                     */
        seq_t val = binom(m, i);       /* C(m,i)                     */
        if (((m - i) & 1) == 1) {      /* multiply by -1 if (m-i) odd */
            val = -val;
        }
        return val;
    } else {                           /* r is odd, r = 2m+1       */
        idx_t m = r >> 1;              /* (r-1)/2                      */
        seq_t val;
        if ((k & 1) == 0) {            /* even column k = 2i         */
            idx_t i = k >> 1;          /* i = k/2                     */
            val = binom(m, i);
            if (((m + i) & 1) == 1) {  /* multiply by -1 if (m+i) odd */
                val = -val;
            }
        } else {                       /* odd column k = 2i+1        */
            idx_t i = (k - 1) >> 1;    /* i = (k-1)/2                */
            val = binom(m, i);
            if (((m + i + 1) & 1) == 1) { /* multiply by -1 if (m+i+1) odd */
                val = -val;
            }
        }
        return val;
    }
}
