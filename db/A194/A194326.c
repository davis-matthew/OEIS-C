/*
 * A194326  Triangular array: g(n,k)=number of fractional parts (i*r) in
 *          interval [(k-1)/n, k/n], for 1<=i<=2n, 1<=k<=n, r=2-sqrt(2).
 * https://oeis.org/A194326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by flattening the triangular array g(r,k)
 *            defined in the OEIS description. r is the row such that
 *            r*(r-1)/2 < n <= r*(r+1)/2, k = n - r*(r-1)/2. g(r,k) = #{ 1 <=
 *            i <= 2r : frac(i*(2-sqrt(2))) in [(k-1)/r , k/r] }
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* locate the row r */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    idx_t offset = r * (r - 1) / 2;
    idx_t k = n - offset;

    /* constant r' = 2 - sqrt(2) */
    static const double Rconst = 2.0 - sqrt(2.0);

    double lower = ((double)(k - 1)) / (double)r;
    double upper = ((double)k) / (double)r;
    const double eps = 1e-12;   /* tolerance for inclusivity */

    seq_t cnt = 0;
    for (idx_t i = 1; i <= 2 * r; ++i) {
        double val = i * Rconst;
        double frac = val - floor(val);
        if (frac + eps >= lower && frac <= upper + eps)
            ++cnt;
    }
    return cnt;
}
