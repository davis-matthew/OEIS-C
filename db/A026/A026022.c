/*
 * A026022  Triangular array T read by rows: T(n,0) = 1 for n >= 0; T(n,k) =
 *          C(n,k) for k = 1,2,...,n, for n = 1,2,3; and for n >= 4, T(n,k) =
 *          T(n-1,k-1) + T(n-1,k) for k = 1,2,...,[ (n+2)/2 ] and T(n,(n+3)/2)
 *          = T(n-1,(n+1)/2) if n is odd.
 * https://oeis.org/A026022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth term of the triangular array T(n,k) read by
 *            rows, where T(n,0)=1, T(n,k)=C(n,k)-C(n,k-4) for 1≤k≤n, and
 *            T(n,n)=1. The triangle is read row by row, skipping the leading
 *            1 for rows n≥5
 * Range:     u8 n <= 34, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for n < 1, but never called */

    idx_t sum = 0;  /* number of terms processed so far */
    for (idx_t r = 0; ; ++r) {
        /* length of row r */
        idx_t len;
        if (r == 0)       len = 1;
        else if (r == 1)  len = 2;
        else if (r == 2)  len = 3;
        else if (r == 3)  len = 4;
        else if (r == 4)  len = 5;
        else {
            idx_t f = r / 2 + 1;           /* floor((r+2)/2) */
            len = f + 1;                   /* k=1..f plus the last k=r */
            if (r & 1) len += 1;           /* odd r adds the special term */
        }

        if (sum + len >= n) {
            /* index falls in this row */
            idx_t offset = n - sum - 1;    /* zero‑based position in the row */

            idx_t k;
            if (r <= 4) {
                k = offset;                /* k runs 0 .. r */
            } else {
                idx_t f = r / 2 + 1;
                if (offset < f) {
                    k = offset + 1;         /* k = 1 .. f */
                } else if ((r & 1) && offset == f) {
                    k = (r + 3) / 2;       /* special odd case */
                } else {
                    /* last element (k = r) */
                    return 1;
                }
            }

            if (k == 0 || k == r) return 1;

            seq_t val = binomial(r, k);
            if (k >= 4) {
                val -= binomial(r, k - 4);
            }
            return val;
        }

        sum += len;
    }
    return 0;  /* unreachable */
}
