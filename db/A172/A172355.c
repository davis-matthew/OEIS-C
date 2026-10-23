/*
 * A172355  Triangle t(n,k) read by rows: generalized Padovan factorial ratios
 *          c(n)/(c(k)*c(n-k)) where c are partial products of a generalized
 *          Padovan sequence with multiplier m=5.
 * https://oeis.org/A172355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) = c(r)/(c(k)*c(r-k)), where r = floor((sqrt(8n+1)-1)/2), k
 *            = n - r*(r+1)/2, c(m) = product_{i=1..m} f(i), f(0)=0,
 *            f(1)=f(2)=1, f(m)=5*f(m-2)+f(m-3) (m≥3). The sequence is the
 *            triangle t(r,k) read by rows
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 45, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* locate the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t k = n - r * (r + 1) / 2;          /* column index in row r */

    /* compute the f sequence up to r */
    /* A small static buffer is safe because r is O(sqrt(n)) (≈141 for n=10000). */
    seq_t f[200];                          /* must be large enough for all r needed */
    f[0] = 0;
    if (r >= 1) f[1] = 1;
    if (r >= 2) f[2] = 1;
    for (idx_t i = 3; i <= r; ++i) {
        f[i] = 5 * f[i - 2] + f[i - 3];
    }

    /* compute numerator = product_{i=k+1..r} f[i] */
    seq_t num = 1;
    for (idx_t i = k + 1; i <= r; ++i) {
        num *= f[i];
    }

    /* compute denominator = product_{i=1..r-k} f[i] */
    seq_t den = 1;
    for (idx_t i = 1; i <= r - k; ++i) {
        den *= f[i];
    }

    /* result is exact */
    return num / den;
}
