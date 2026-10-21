/*
 * A080521  Triangle read by rows; n-th row contains the lexicographically
 *          first set of n distinct positive integers whose sum is 2^n.
 * https://oeis.org/A080521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 152, u32 n <= 560, u64 n <= 2144,
 *            u128 n <= 8384, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A080521 – Triangle read by rows.
 *  a(n) is the n‑th element of the triangle whose rows contain the
 *  lexicographically first set of n distinct positive integers that sum
 *  to 2^n.  For the row r (r ≥ 1) the entries are:
 *      T(r, k) = k               for 1 ≤ k < r
 *      T(r, r) = 2^r – r(r‑1)/2
 *  a(n) enumerates the triangle row‑by‑row (1‑based indexing).
 *  For n < 1 we return 0.  The function works for any seq_t (8‑,16‑,
 *  32‑, 64‑, 128‑bit unsigned or arbitrary–precision integer type).
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* out of offset */

    /* find the row r such that r(r+1)/2 ≥ n */
    idx_t r = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    while (r * (r + 1) / 2 < n)
        ++r;                              /* adjust for rounding */

    /* index of element within the row (1‑based) */
    idx_t prev_terms = r * (r - 1) / 2;
    idx_t k = n - prev_terms;             /* 1 ≤ k ≤ r */

    if (k < r)                            /* off‑diagonal entry */
        return (seq_t)k;

    /* diagonal entry: 2^r – r(r‑1)/2 */
    seq_t two_pow_r = ((seq_t)1 << r);     /* 1 << r works for all supported seq_t */
    seq_t sub = ((seq_t)r * (seq_t)(r - 1)) / 2;
    return two_pow_r - sub;
}
