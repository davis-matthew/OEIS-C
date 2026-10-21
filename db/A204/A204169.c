/*
 * A204169  Array: row n shows the coefficients of the characteristic
 *          polynomial of the n-th principal submatrix of (i+j-1), as in
 *          A002024.
 * https://oeis.org/A204169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the coefficient obtained by scanning row‑major the
 *            coefficients of the characteristic polynomial of the n‑th
 *            principal submatrix of (i+j‑1). The polynomial of size r is
 *            det((i+j‑1) – x⋅I_r) = x^r – r² x^(r‑1) – (r²(r²‑1)/12) x^(r‑2)
 *            (for r≥2), with signs flipped for odd r. The coefficient...
 * Range:     i8 n <= 32, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
typedef uint64_t idx_t;

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset is 1 */

    /* Find the row r containing the n-th term.
       Row r has r+1 coefficients; cumulative terms up to row r:
          S(r) = r(r+3)/2.  */
    idx_t low = 0, high = 1;
    while (((__int128)high * (high + 3) / 2) < n) high <<= 1;

    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 val = (__int128)mid * (mid + 3) / 2;
        if (val >= n) high = mid;
        else          low  = mid + 1;
    }
    idx_t r = low;  /* row number (>=1) */

    /* Number of terms in all preceding rows */
    __int128 prevS = (__int128)(r - 1) * (r + 2) / 2;
    idx_t c = n - (idx_t)prevS - 1;       /* column index in this row (0‑based) */

    /* Special handling for the first row */
    if (r == 1) {
        return (c == 0) ? (seq_t)1 : (seq_t)(-1);
    }

    /* For r ≥ 2 the polynomial has non‑zero coefficients only for
       x^r, x^(r‑1) and x^(r‑2). */
    if (c == r) {                         /* coefficient of x^r */
        return (r % 2 == 0) ? (seq_t)1 : (seq_t)(-1);
    }
    if (c == r - 1) {                     /* coefficient of x^(r‑1) */
        seq_t val = (seq_t)r * (seq_t)r;   /* r² */
        return (r % 2 == 0) ? -val : val;
    }
    if (c == r - 2) {                     /* coefficient of x^(r‑2) */
        seq_t sq = (seq_t)r * (seq_t)r;       /* r² */
        seq_t v  = sq * (sq - 1) / 12;        /* r²(r²‑1)/12 */
        return (r % 2 == 0) ? -v : v;          /* sign = (−1)^(r+1) */
    }

    /* All other positions are zero */
    return (seq_t)0;
}
