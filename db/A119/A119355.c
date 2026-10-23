/*
 * A119355  Triangle generated from binomial transforms of (1; 2, 3; 3, 4, 5;
 *          ...).
 * https://oeis.org/A119355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th row‑major element of the triangle T(n,k)
 *            defined by T(r,k) = Σ_{j=0}^{r-k} (r-k+1+j)·C(k-1,j), where r =
 *            ⌈(√(8n+1)-1)/2⌉ and k = n - (r-1)r/2. The triangle is read row
 *            by row: 1; 2,1; 3,5,1; 4,7,8,1; …
 * Range:     u8 n <= 51, u16 n <= 223, u32 n <= 932, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* offset 1; values for n>=1 defined */

    /* Find the row r such that r(r+1)/2 >= n and (r-1)r/2 < n */
    idx_t low = 0, high = 2 * n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri < n) low = mid + 1;
        else high = mid;
    }
    idx_t r = low;                         /* current row number */

    /* Position of n within row r (0‑based) */
    idx_t r_prev = r - 1;
    unsigned __int128 tri_prev = (unsigned __int128)r_prev * (r_prev + 1) / 2;
    idx_t offset = (idx_t)(n - (idx_t)tri_prev) - 1;   /* 0‑based */
    idx_t k = offset + 1;                      /* column number (1‑based) */

    /* Compute the triangle entry T(r,k) */
    idx_t rem = r + 1 - k;                   /* r-k+1 */
    seq_t sum = 0;
    for (idx_t j = 0; j < rem; ++j) {
        sum += (seq_t)(rem + j) * binomial(k - 1, j);
    }
    return sum;
}
