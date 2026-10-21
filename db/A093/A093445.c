/*
 * A093445  The triangular triangle.
 * https://oeis.org/A093445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, tabl
 * Formula:   a(n) = TT(r,k) * r is the row such that r(r+1)/2 ≥ n * k = n –
 *            r(r-1)/2 (1 ≤ k ≤ r) * TT(r,k) = T(k*r - T(k-1)) – T((k-1)*r -
 *            T(k-2)), * where T(x) = x(x+1)/2 and TT(r,1)=TT(r,r)=T(r). * The
 *            sequence is the flattened triangle row‑by‑row starting at a(1)=1
 * Range:     u8 n <= 58, u16 n <= 2439, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Triangular number: T(x) = x(x+1)/2, computed in seq_t. */
static seq_t tri(seq_t x)
{
    return x * (x + 1) / 2;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = 1;
    while ((seq_t)r * (r + 1) / 2 < n)
        ++r;

    /* Number of terms before this row. */
    idx_t prev_cum = (r - 1) * r / 2;

    /* Position k within the row (1‑based). */
    idx_t k = n - prev_cum;          /* 1 ≤ k ≤ r */

    /* Special cases: first and last entry of a row. */
    if (k == 1 || k == r)
        return tri((seq_t)r);

    /* Compute the required indices in seq_t. */
    seq_t rn   = (seq_t)r;
    seq_t ks   = (seq_t)k;
    seq_t km1  = (seq_t)(k - 1);
    seq_t km2  = (seq_t)(k - 2);

    /* idx1 = k*r - T(k-1), idx2 = (k-1)*r - T(k-2) */
    seq_t idx1 = ks * rn - tri(km1);
    seq_t idx2 = km1 * rn - tri(km2);

    /* Final value: T(idx1) - T(idx2). */
    return tri(idx1) - tri(idx2);
}
