/*
 * A194298  Triangular array: g(n,k)=number of fractional parts (i*r) in
 *          interval [(k-1)/n, k/n], for 1<=i<=2n, 1<=k<=n, r=(1+sqrt(3))/2.
 * https://oeis.org/A194298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = g(row, col) * where row, col are taken from the flattened
 *            row‑wise * enumeration of the triangular array with entries * *
 *            g(n,k) = #{ 1 ≤ i ≤ 2n : * frac(i * α) ∈ [(k-1)/n , k/n] } * * α
 *            = (1+√3)/2. The value is computed by a direct * iteration over
 *            the 2r terms of the row. For n up * to 1...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>  /* sqrt */

static const double alpha = (1.0 + sqrt(3.0)) / 2.0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* safety for out‑of‑range calls */

    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    while ( (unsigned __int128)r * (r + 1) / 2 < n ) ++r;

    /* Number of terms before row r. */
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - prev;   /* column in [1, r] */

    /* Count the i‑satisfying the interval. */
    seq_t count = 0;
    double low  = (double)(k - 1) / (double)r;
    double high = (double)k / (double)r;
    const double eps = 1e-12;   /* guard against round‑off */

    for (idx_t i = 1; i <= 2 * r; ++i) {
        double x = i * alpha;
        double frac = x - floor(x);
        if (frac + eps >= low && frac - eps <= high) {
            ++count;
        }
    }

    return count;
}
