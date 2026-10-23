/*
 * A157169  Triangle, read by rows, T(n,k,m) = (m*(n-k)+1)*binomial(n-1, k-1)
 *          + (m*k+1)* binomial(n-1, k) + m*k*(n-k)*binomial(n-2, k-1), with
 *          m=1.
 * https://oeis.org/A157169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 111, u32 n <= 447, u64 n <= 1856,
 *            u128 n <= 5150, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A157169
 *
 *  Triangle, read by rows, with m = 1:
 *      T(n,k) = (n-k+1)*C(n-1,k-1) + (k+1)*C(n-1,k) + k*(n-k)*C(n-2,k-1)
 *  The sequence a(n) is obtained by listing the triangle row by row,
 *  starting with a(0) = 1.
 *
 *  Offset: 0   (a(0) is the first term)
 */

#include "../utils/binomial.h"

/* Return C(n,k) if 0 <= k <= n, otherwise 0 */
static inline seq_t safe_binom(idx_t n, idx_t k)
{
    return (k > n) ? 0 : binomial(n, k);
}

seq_t a(idx_t n)
{
    /* Find the row r (0‑based) that contains index n, and the
     * starting index of that row.  The number of entries in
     * rows 0..r-1 is r*(r+1)/2, so we find the smallest r with
     * r*(r+1)/2 > n. */
    idx_t r = 0;
    idx_t start = 0;          /* first index of current row */
    while (start + r + 1 <= n) {
        start += r + 1;
        r++;
    }

    /* Column index within the row (0‑based) */
    idx_t k = n - start;

    /* Handle the trivial first row */
    if (r == 0) return 1;

    /* Compute the three terms of the triangle entry */
    seq_t term1 = 0, term2, term3 = 0;

    /* First term requires k ≥ 1 */
    if (k >= 1) {
        term1 = ((seq_t)(r - k + 1)) * safe_binom(r - 1, k - 1);
    }

    /* Second term is valid for all 0 ≤ k ≤ r */
    term2 = ((seq_t)(k + 1)) * safe_binom(r - 1, k);

    /* Third term requires r ≥ 2 and k ≥ 1 */
    if (r >= 2 && k >= 1) {
        term3 = ((seq_t)k) * ((seq_t)(r - k)) * safe_binom(r - 2, k - 1);
    }

    return term1 + term2 + term3;
}
