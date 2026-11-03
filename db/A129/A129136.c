/*
 * A129136  Permutations with exactly 6 fixed points.
 * https://oeis.org/A129136
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 195 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A129136 – permutations with exactly 6 fixed points
 * Formula: a(n) = C(n,6) * D_{n-6}
 * where D_k is the number of derangements (subfactorials).
 * The derangement values are computed iteratively, so no dynamic
 * allocation is required, and the implementation works for both
 * built‑in integer types and arbitrary‑precision integer classes.
 */

#include <stdlib.h>

/* compute D_k, the k‑th derangement (subfactorial) */
static seq_t derange(idx_t k)
{
    if (k == 0) return (seq_t)1;
    if (k == 1) return (seq_t)0;

    seq_t d0 = (seq_t)1;   /* D_0 */
    seq_t d1 = (seq_t)0;   /* D_1 */
    for (idx_t i = 2; i <= k; ++i) {
        seq_t di = ((seq_t)(i - 1)) * (d1 + d0);
        d0 = d1;
        d1 = di;
    }
    return d1;
}

/* sequence term a(n) for n >= 6 */
seq_t a(idx_t n)
{
    if (n < 6) return (seq_t)0;  /* below offset – undefined */

    /* compute C(n,6) = n*(n-1)*...*(n-5)/720 */
    seq_t comb = (seq_t)n;
    for (idx_t i = 1; i <= 5; ++i) {
        comb *= (seq_t)(n - i);
    }
    comb /= (seq_t)720;  /* 6! */

    /* compute D_{n-6} */
    seq_t d = derange(n - 6);

    /* return the product */
    return comb * d;
}
