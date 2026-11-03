/*
 * A109264  Riordan array (1-x-x^2,x(1-x)).
 * https://oeis.org/A109264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   Riordan array (1-x-x^2, x(1-x)) T(r,k) = [x^r]
 *            (1-x-x^2)*(x(1-x))^k a(n) = T(r, k) where r = row_of_index(n)
 *            and k = n - r(r+1)/2 (start index of row r)
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static inline idx_t row_of_index(idx_t n)
{
    /* Binary search for the largest r with r*(r+1)/2 <= n. */
    idx_t low = 0, high = n, mid, tri;
    idx_t r = 0;
    while (low <= high)
    {
        mid = low + (high - low) / 2;
        tri = mid * (mid + 1) / 2;
        if (tri <= n) { r = mid; low = mid + 1; }
        else           high = mid - 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Determine the row r and the column k of the triangle element. */
    idx_t r = row_of_index(n);

    /* Compute the starting index of row r without risking overflow. */
    idx_t s;
    if (r & 1)               /* r is odd: (r+1)/2 * r */
        s = ((r + 1) / 2) * r;
    else                     /* r is even: r/2 * (r+1) */
        s = (r / 2) * (r + 1);

    idx_t k = n - s;          /* column index (0‑based) */
    idx_t diff = r - k;       /* r - k  (>= 0 because k <= r) */

    seq_t result = 0;

    /* Offsets 0,1,2 correspond to the powers k+i, k+i+1, k+i+2
       in the expansion of (1-x-x^2)(x(1-x))^k. */
    for (idx_t o = 0; o <= 2; ++o) {
        if (o > diff) break;             /* no non‑negative i possible */
        idx_t i = diff - o;              /* i = r - k - o */
        if (i > k) continue;             /* i must not exceed k */

        seq_t val = binomial(k, i);   /* binomial coefficient C(k,i) */

        if (o == 0) {                    /* coefficient +(-1)^i */
            if (i & 1)                   /* i odd → subtract */
                result -= val;
            else
                result += val;
        } else {                         /* coefficient –(-1)^i (for o=1,2) */
            if (i & 1)                   /* i odd → add */
                result += val;
            else                         /* i even → subtract */
                result -= val;
        }
    }

    return result;
}
