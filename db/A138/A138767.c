/*
 * A138767  Triangle read by rows: T(n,k) =
 *          binomial(n,k)*binomial(2*n-2*k,n-1), n>=1, 0<=k<=floor(n/2+1/2).
 * https://oeis.org/A138767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the linearized triangle T(r,k) where T(r,k) =
 *            binomial(r,k) * binomial(2*r-2*k, r-1). For n = 1,2,3,4,... the
 *            sequence starts : 1,1,4,4,15,18,3,
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* Offset is 1; n>=1 in the sequence */

    /* Convert the linear index into (row, column) indices. */
    idx_t pos = n - 1;              /* 0‑based linear position */
    idx_t r   = 1;                  /* current row */

    for (;;) {
        /* Row length: when r is even -> r/2 + 1,
           when r is odd  -> (r-1)/2 + 2. */
        idx_t len = (r & 1) ? ((r - 1) / 2 + 2) : (r / 2 + 1);

        if (pos < len) break;      /* found the row */
        pos -= len;
        ++r;
    }

    idx_t k = pos;                 /* column index (0‑based) within the row */

    /* Compute the binomial terms using the helper. */
    seq_t term = binomial(r, k);
    term *= binomial(2 * r - 2 * k, r - 1);

    return term;
}
