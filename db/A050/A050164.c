/*
 * A050164  T(n,k)=S(2n+3,n+3,k+3), 0<=k<=n, n >= 0, array S as in A050157.
 * https://oeis.org/A050164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = floor((sqrt(8*n+1)-1)/2) (row index) *
 *            k = n - r*(r+1)/2 (column index in that row) * T(r,k) =
 *            Σ_{j=0..k} [C(2r+3, r-j) - C(2r+3, r-1-j)] * (C(.,.) denotes the
 *            binomial coefficient) * This follows from the definition
 *            T(n,k)=S(2n+3,n+3,k+3) and * T(n,k)=Σ_{j=0..k}t(n,j) w...
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* locate row r */
    idx_t r = 0;
    for (;; ++r) {
        /* use 128‑bit to avoid overflow */
        __int128 total = (__int128)(r + 1) * (r + 2) / 2;
        if (total > n)
            break;
    }

    /* number of terms before row r */
    __int128 pre128 = (__int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)pre128;          /* column index within the row */

    /* compute the partial sum T(r,k) */
    seq_t sum = 0;
    idx_t N = 2 * r + 3;                  /* fixed top of binomial */

    for (idx_t j = 0; j <= k; ++j) {
        idx_t k1 = r - j;                 /* first binomial argument */
        seq_t term1 = binomial(N, k1);    /* C(N, r-j) */

        seq_t term2 = 0;
        if (r > 0) {                     /* second binomial; skip if r==0 */
            idx_t k2 = r - 1 - j;         /* C(N, r-1-j) where r-1-j>=0 */
            term2 = binomial(N, k2);
        }

        sum += (term1 - term2);
    }

    return sum;
}
