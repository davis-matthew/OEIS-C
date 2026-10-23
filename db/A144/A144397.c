/*
 * A144397  Triangle read by rows: t(n,m)=(1 + n!)*Binomial[n,
 *          m]-n!/Binomial[n, m].
 * https://oeis.org/A144397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows:  t(r,p) = (1 + r!)*C(r,p) – r!/C(r,p)
   The sequence a(n) is obtained by writing the rows of t for
   r = 0,1,2,… side by side, starting at n = 1. */

#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1, harmless guard */

    /* Find the row r such that r(r+1)/2 < n ≤ (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 < n)   /* r stays small (≤ 150 for n≤10⁴) */
        ++r;

    idx_t start = r * (r + 1) / 2 + 1;   /* first index of this row */
    idx_t pos   = n - start;             /* position within the row (0‑based) */

    if (pos == 0 || pos == r)           /* the two borders are always 1 */
        return 1;

    seq_t fact = factorial(r);
    seq_t C    = binomial(r, pos);

    /* (1 + r!)*C(r,p) – r!/C(r,p) */
    seq_t result = (fact + 1) * C - fact / C;
    return result;
}
