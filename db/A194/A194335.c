/*
 * A194335  Triangular array: g(n,k)=number of fractional parts (i*r) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n^2, 1<=k<=n, r=2-phi, where
 *          phi=(1+sqrt(5))/2, the golden ratio.
 * https://oeis.org/A194335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A194335
 *  Triangular array: g(n,k)=number of fractional parts (i*r) in interval
 *  [(k-1)/n, k/n], for 1<=i<=n^2, 1<=k<=n,  r=2-φ, φ=(1+√5)/2.
 *  The sequence a(n) is the flattened array row‑by‑row (row 1 has 1
 *  element, row 2 has 2, …). This implementation computes a(n) by
 *  determining the corresponding row nrow and column kcol, then
 *  counting the fractional parts of i*r that lie in the appropriate
 *  sub‑interval.
 */
#include <stddef.h>

static const double R = 0.3819660112501051;  /* r = 2 – φ */

/* seq_t and idx_t are supplied by the build system.  */
seq_t a(idx_t n)
{
    /* Find the row number r such that r*(r+1)/2 >= n.  */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n) ++r;

    /* Column index within the row. */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;          /* 1 <= k <= r */

    /* Lower and upper bounds of the k‑th subinterval. */
    double lower = (double)(k - 1) / (double)r;
    double upper = (double)k / (double)r;

    /* Count fractional parts falling into [(k-1)/r, k/r).  */
    idx_t count = 0;
    for (idx_t i = 1; i <= r * r; ++i) {
        double val   = (double)i * R;
        double frac  = val - (double)((idx_t)val);   /* fractional part */
        if (frac >= lower && frac < upper) ++count;
    }

    return (seq_t)count;
}
