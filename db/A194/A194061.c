/*
 * A194061  Natural interspersion of A002620; a rectangular array, by
 *          antidiagonals.
 * https://oeis.org/A194061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A194061: Natural interspersion of A002620; a rectangular array,
 * enumerated by antidiagonals (i+j constant).
 * The algorithm 
 *   1. Find the smallest diagonal d such that d(d+1)/2 ≥ n.
 *   2. Compute the offset inside that diagonal: offset = n - (d-1)d/2.
 *   3. The coordinates are i = offset, j = d - offset + 1.
 *   4. The entry a(i,j) is
 *        • 1 if i=j=1.
 *        • first_row(j) = 1 + Σ_{t=2}^{j} ⌊(1+t)/2⌋   (i=1, j>1)
 *        • first_col(i) = i² – 1                      (j=1, i>1)
 *        • for i>1, j>1:
 *                a(i,j) = i²-1 + (j-1)i + Σ_{t=2}^{j} ⌊(t-2)/2⌋
 *   The sum Σ_{t=2}^{j} ⌊(t-2)/2⌋ equals ⌊(j-2)/2⌋² + (j-2 mod 2)·⌊(j-2)/2⌋.
 * The function works for any seq_t/idx_t types, returns the correct value
 * for all n≥1 and is O(log n) per call.
 */

#include <stdint.h>   /* only for standard types; idx_t and seq_t are defined by the build */

static seq_t first_col(idx_t i)
{
    /* a(i,1) = i^2 - 1   for i ≥ 2  */
    seq_t ii = (seq_t)i;
    return ii * ii - (seq_t)1;
}

static seq_t first_row(idx_t j)
{
    /* a(1,j) = 1 + Σ_{t=2}^{j} ⌊(1+t)/2⌋  */
    seq_t val = (seq_t)1;
    for (idx_t t = 2; t <= j; ++t)
        val += (seq_t)((1 + t) / 2);
    return val;
}

static seq_t value_ij(idx_t i, idx_t j)
{
    /* a(i,j) for i > 1 and j > 1  */
    seq_t ii = (seq_t)i;
    seq_t val = ii * ii - (seq_t)1;          /* i²-1 */

    val += ((seq_t)(j - 1)) * ii;           /* (j-1)·i */

    /* Sum over t=2..j of floor((t-2)/2) */
    idx_t k = j - 2;
    idx_t m = k / 2;                        /* ⌊k/2⌋ */
    idx_t r = k % 2;                        /* k mod 2 */
    seq_t mm = (seq_t)m;
    val += mm * mm + (seq_t)r * mm;         /* ⌊k/2⌋² + (k mod 2)·⌊k/2⌋ */

    return val;
}

seq_t a(idx_t n)
{
    /* 1. Find diagonal d   (smallest d s.t. d(d+1)/2 ≥ n) */
    idx_t lo = (idx_t)1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t d = lo;

    /* 2. Offset within the diagonal */
    unsigned __int128 before = (unsigned __int128)(d - 1) * d / 2;
    idx_t offset = (idx_t)(n - before);      /* 1-based offset */

    /* 3. Coordinates (i,j) */
    idx_t i = offset;
    idx_t j = d - offset + 1;

    /* 4. Compute the value */
    if (i == 1)
        return first_row(j);
    if (j == 1)
        return first_col(i);
    return value_ij(i, j);
}
