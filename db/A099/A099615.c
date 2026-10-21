/*
 * A099615  Triangle read by rows, 2<=k<=n: T(n,k) = denominator of
 *          (1+1/n)^k-(1+k/n) and of (1-1/n)^k-(1-k/n).
 * https://oeis.org/A099615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   big_pow, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A099615 – Triangle read by rows: T(n,k) = n^k for 2≤k≤n
 * Sequence entry a(n) is the nth element of the triangle read row‑wise.
 * Formula:
 *     a(n) = r^k
 * where r is the unique row ≥2 such that r(r−1)/2 ≥ n,
 * k = 2 + (n – (r−1)(r−2)/2 – 1).
 */
#include "../utils/floor_sqrt.h"
#include "../utils/big_pow.h"

/* Compute the nth term of A099615 (1‑based index). */
seq_t a(idx_t n)
{
    /* Convert index to sequence type for safe arithmetic. */
    seq_t idx = (seq_t)n;

    /* Discriminant of the quadratic r(r−1)/2 ≥ idx:
     *   r^2 − r − 2idx ≥ 0  -->  r ≈ (1 + sqrt(1+8idx)) / 2.
     */
    seq_t d = (seq_t)1 + (seq_t)8 * idx;   /* 1 + 8*n  (in seq_t) */
    seq_t s = floor_sqrt(d);              /* floor sqrt of discriminant */

    /* Initial estimate of r.  It may be one less than the real answer. */
    seq_t r_seq = ((seq_t)1 + s) / (seq_t)2;
    idx_t r = (idx_t)r_seq;

    /* Correct r if the inequality is not yet satisfied. */
    while (((seq_t)r * (seq_t)(r - 1)) / (seq_t)2 < idx)
        ++r;

    /* Number of terms before row r: (r−1)(r−2)/2. */
    seq_t terms_before = ((seq_t)(r - 1) * (seq_t)(r - 2)) / (seq_t)2;

    /* 0‑based offset within the row. */
    seq_t offset = idx - terms_before - (seq_t)1;

    /* Column index k = 2 + offset. */
    seq_t k = offset + (seq_t)2;

    /* Return r^k. */
    return big_pow((seq_t)r, k);
}
