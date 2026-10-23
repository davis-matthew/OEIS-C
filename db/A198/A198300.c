/*
 * A198300  Square array M(k,g), read by antidiagonals, of the Moore lower
 *          bound on the order of a (k,g)-cage.
 * https://oeis.org/A198300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 144, u32 n <= 369, u64 n <= 1065,
 *            u128 n <= 3057, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) = M(k,g)  where
 *   M(k,2i)   = 2 * Σ_{j=0}^{i-1} (k-1)^j
 *   M(k,2i+1) = (k-1)^i + 2 * Σ_{j=0}^{i-1} (k-1)^j
 *
 * The indices k,g are obtained by reading the square matrix M
 * by antidiagonals.  For a given n (offset 1) the diagonal
 * index m satisfies m*(m+1)/2 ≥ n.  Then S = m+4, 
 * k = S - offset - 2, g = S - k.
 *
 * This implementation computes the sums iteratively, avoiding
 * excess intermediate values that would overflow a 64‑bit build.
 */
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe fallback */

    /* Find the diagonal index m (m ≥ 1) */
    idx_t m = (idx_t)((sqrt((double)n * 8 + 1) - 1) / 2);
    while (((m * (m + 1)) / 2) < n) ++m;

    /* Previous cumulative count */
    idx_t prev = ((m - 1) * m) / 2;
    idx_t offset = n - prev;          /* 1‑based position on this diagonal */

    idx_t S = m + 4;                  /* k + g for this diagonal   */
    idx_t k = S - offset - 2;         /* k  ( decreases along the diagonal ) */
    idx_t g = S - k;                  /* g */

    /* Base for the geometric series */
    seq_t base = (seq_t)k - 1;
    idx_t i = g / 2;                  /* floor(g/2) */

    /* Compute the sum Σ_{j=0}^{i-1} base^j iteratively */
    seq_t sum  = 0;
    seq_t term = 1;                    /* base^0 */
    for (idx_t j = 0; j < i; ++j) {
        sum += term;
        term *= base;                  /* term becomes base^(j+1) */
    }

    /* term now equals base^i (used only for odd g) */
    if ((g & 1) == 0) {               /* even g  ->  M = 2*sum */
        return sum + sum;
    } else {                           /* odd g   ->  M = base^i + 2*sum */
        return term + sum + sum;
    }
}
