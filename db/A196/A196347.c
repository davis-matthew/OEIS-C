/*
 * A196347  Triangle T(n, k) read by rows, T(n, k) = n!*binomial(n, k).
 * https://oeis.org/A196347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(floor((sqrt(1+8n)-1)/2), n -
 *            floor((sqrt(1+8n)-1)/2)*(floor((sqrt(1+8n)-1)/2)+1)/2 ) where
 *            T(r,k)=r!*C(r,k)
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 68, u64 n <= 174,
 *            u128 n <= 446, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    // Determine the row r.
    idx_t r = (idx_t)((sqrt(1.0 + 8.0 * (double)n) - 1.0) / 2.0);
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    // Compute factorial(r).
    seq_t fact = 1;
    for (idx_t i = 2; i <= r; ++i)
        fact *= (seq_t)i;

    // Compute C(r,k) multiplicatively.
    seq_t binom = 1;
    if (k > 0) {
        idx_t kk = k;
        if (kk > r - kk)
            kk = r - kk;  // use symmetry
        for (idx_t i = 1; i <= kk; ++i) {
            binom = binom * (seq_t)(r - kk + i);
            binom = binom / (seq_t)i;
        }
    }

    return fact * binom;
}
