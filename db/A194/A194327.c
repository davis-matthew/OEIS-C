/*
 * A194327  Triangular array: g(n,k)=number of fractional parts (i*r) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n^2, 1<=k<=n, r=2-sqrt(2).
 * https://oeis.org/A194327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of i in [1, r²] whose fractional part of i*(2–√2)
 *            lies in the interval [(k–1)/r, k/r), where r and k are obtained
 *            from n by r = ceil( (√(8n–7)–1)/2 ) and k = n – r(r–1)/2
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Find row r so that r*(r+1)/2 >= n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* Column k (1‑based) */
    idx_t k = n - r * (r - 1) / 2;

    /* Constant r = 2 - sqrt(2) */
    static const double r_const = 2.0 - sqrt(2.0);

    /* Interval boundaries in double */
    double low  = (double)(k - 1) / (double)r;
    double high = (double)k / (double)r;

    seq_t count = 0;
    for (idx_t i = 1; i <= r * r; ++i)
    {
        double x     = i * r_const;
        double frac  = x - floor(x);          /* fractional part of i*r_const */
        if (frac >= low && frac < high)
            ++count;
    }
    return count;
}
