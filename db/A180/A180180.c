/*
 * A180180  Triangle read by rows: T(n,k) is the number of compositions of n
 *          without 5's and having k parts; 1 <= k <= n.
 * https://oeis.org/A180180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A180180 – number of compositions of r into k parts all
    different from 5.  The sequence is the triangle read by rows:
    T(r,k) is defined for 1 ≤ k ≤ r; a(n) is the nth element in the
    flattened list of the rows.  This function computes a(n) for all
    n ≥ 1 by first determining the row r and column k from the
    triangular indexing, then evaluating the inclusion‑exclusion
    formula
    T(r,k)=∑_{i=0}^{k} (‑1)^i * C(k,i) * C(r‑5*i‑1, k‑i‑1),
    where C(n,k)=0 if k<0 or k>n and C(0,0)=1.  A helper
    ways_to_partition(m,r) = C(m‑1,r‑1) for m≥r≥1 and
    ways_to_partition(0,0)=1 apart from those cases the value is 0.
*/

#include <math.h>
#include <stdint.h>

/* compute binomial coefficient C(n,k) for small n (≤ 200) as seq_t.
   Returns 0 if k<0 or k>n.  For seq_t types supporting / and * this
   works for arbitrary precision as well. */
static seq_t binom_uint(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;           /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result *= (seq_t)(n - k + i);
        result /= (seq_t)i;
    }
    return result;
}

/* number of compositions of m into r parts (each ≥1).  For m=0,r=0
   there is one empty composition; otherwise it equals C(m‑1,r‑1) when
   m≥r≥1, else 0. */
static seq_t ways_to_partition(idx_t m, idx_t r)
{
    if (r == 0) return (m == 0) ? 1 : 0;
    if (m < r) return 0;
    if (r == 1) return 1;                     /* C(m‑1,0)=1 */
    return binom_uint(m - 1, r - 1);
}

/* a(n) – t‑th element of flattened triangle of A180180. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* sequence starts at 1 */

    /* determine row r such that r(r+1)/2 ≥ n  (triangular numbers) */
    idx_t r = (idx_t)((sqrt((double)(1 + 8.0 * (double)n)) - 1.0) / 2.0);
    while (r * (r + 1) / 2 < n) ++r;           /* adjust for rounding errors */

    /* column k within that row */
    idx_t k = n - (r - 1) * r / 2;

    /* inclusion‑exclusion over i parts that would be equal to 5 */
    const idx_t p = 5;
    seq_t pos = 0, neg = 0;                    /* accumulate positive/negative */
    for (idx_t i = 0; i <= k; ++i) {
        /* remaining sum after fixing i parts to 5 */
        idx_t sum_rem = r;
        if (p * i > r) continue;              /* n‑p*i < 0 ⇒ impossible */
        sum_rem -= p * i;

        /* remaining parts to be filled */
        idx_t parts_rem = k - i;

        /* compute contribution */
        seq_t ways = ways_to_partition(sum_rem, parts_rem);
        if (ways == 0) continue;

        seq_t coeff = binom_uint(k, i);
        seq_t term = coeff * ways;

        if ((i & 1U) == 0) pos += term;       /* even i → positive sign   */
        else                neg += term;      /* odd i  → negative sign   */
    }

    /* pos ≥ neg is guaranteed by inclusion‑exclusion */
    if (pos < neg) return 0;                   /* safety for pathological builds */
    return pos - neg;
}
