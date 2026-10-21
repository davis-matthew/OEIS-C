/*
 * A210042  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A124927; see the Formula section.
 * https://oeis.org/A210042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Let n≥1 and let i = n-1 be the zero‑based index. Find the unique
 *            r such that i ≤ r(r+1)/2 (truncated down to the row where the
 *            term lies). Then the row length is 1 for r=0,1 and r for r≥2.
 *            Let d = i - ((r-1)r/2 + 1) be the offset within that row. The
 *            term a(n) is if d == 0 → 2*r + 1 else → 2 * bi...
 * Range:     u8 n <= 49, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Local binomial coefficient for a possibly large 64‑bit or arbitrary
   precision integer type.  The intermediate computations are performed
   in unsigned __int128 to avoid overflow for the 64‑bit build. */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        __int128 tmp = (__int128)res * (n - k + i);
        res = (seq_t)(tmp / i);        /* division exact */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* not used – offset is 1 */
    idx_t i = n - 1;                   /* zero‑based index */

    /* Find the row r such that i ≤ r(r+1)/2 */
    idx_t r = 0;
    while (1) {
        __int128 tri = (__int128)r * (r + 1) / 2;
        if (tri >= i) break;
        ++r;
    }

    /* Starting index of this row (zero‑based) */
    idx_t start = (r == 0) ? 0
                          : (unsigned __int128)(r - 1) * r / 2 + 1;

    idx_t offset = i - start;            /* position within the row */

    if (offset == 0) {
        return (seq_t)(2 * r + 1);
    } else {
        /* For r ≥ 2 this will be called; the binomial coefficient
           handles the trivial case automatically. */
        seq_t val = binom_seq(r, offset + 1);
        return val * (seq_t)2;
    }
}
