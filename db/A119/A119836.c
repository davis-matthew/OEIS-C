/*
 * A119836  Bi-diagonal inverse of [k<=n]*n!/(2k)!.
 * https://oeis.org/A119836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 18, i32 n <= 42, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A119836: Bi-diagonal inverse of 
   T(n,k) = (2n)!/n!      if k = n
             -(2n)!/(n-1)! if k = n-1
             0             otherwise.
   a(n) enumerates L[n,k] row‑wise:
       r = floor((sqrt(8n+1)-1)/2)
       c = n - r(r+1)/2
       if c == r          a(n) = (2r)!/r!
       else if c == r-1   a(n) = -(2r)!/(r-1)!
       else               a(n) = 0
   (Implementation uses only seq_t arithmetic.)
*/

#include <stdint.h>     /* optional, for clarity */

static inline idx_t triangular(idx_t r) {
    /* returns r*(r+1)/2 */
    return r * (r + 1) / 2;
}

seq_t a(idx_t n) {
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0, t = 0;
    while (t + r + 1 <= n) {
        ++r;
        t = triangular(r);
    }
    idx_t c = n - t;            /* offset inside row r */

    if (c == r) {                       /* main diagonal */
        seq_t val = 1;
        for (idx_t i = r + 1; i <= 2 * r; ++i)
            val = val * (seq_t)i;
        return val;
    }
    if (c == r - 1 && r > 0) {          /* sub‑diagonal */
        seq_t val = 1;
        for (idx_t i = r; i <= 2 * r; ++i)
            val = val * (seq_t)i;
        return -val;
    }
    return 0;                          /* all other entries */
}
