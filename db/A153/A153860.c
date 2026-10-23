/*
 * A153860  Triangle by columns: leftmost column = (1, 0, 1, -1, 1, -1, 1,
 *          ...); columns >1 = (1, 1, 0, 0, 0, ...).
 * https://oeis.org/A153860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 66, i16 n <= 5050, i32 n <= 5050, i64 n <= 5050,
 *            i128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Sequence A153860:
 *  Flattening of the lower triangular matrix M where:
 *  - M[i][1] = 1 (i=1), 0 (i=2), 1 if i odd else -1 (i≥3).
 *  - For j≥2, M[i][j] = 1 if i-j < 2 else 0.
 *  a(n) is the entry at position n in the row‑wise enumeration, starting with a(1)=1.
 */

/* Find the row index r such that T_{r-1} < n <= T_r, where T_k = k(k+1)/2. */
static idx_t triangular_row(idx_t n)
{
    seq_t N = (seq_t)n;                /* truncate n to seq_t for comparison */
    idx_t lo = 1;
    idx_t hi = 1;

    /* Grow hi until its triangular number reaches or exceeds N. */
    while (((seq_t)hi * (hi + 1)) / 2 < N)
        hi <<= 1;                      /* double hi */

    /* Binary search between lo and hi. */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        seq_t T = ((seq_t)mid * (mid + 1)) / 2;
        if (T >= N)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;                          /* r = lo */
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;         /* undefined for n < 1, return 0 */

    idx_t r = triangular_row(n);        /* row index (1‑based) */
    idx_t prev = ((idx_t)(r - 1) * r) / 2;   /* T_{r-1} */
    idx_t col = n - prev;                   /* column index within row */

    if (col == 1) {                       /* leftmost column */
        if (r == 1) return (seq_t)1;
        if (r == 2) return (seq_t)0;
        return ((r & 1) ? (seq_t)1 : (seq_t)(-1));
    } else {                              /* columns j >= 2 */
        return ((col == r || col == r - 1) ? (seq_t)1 : (seq_t)0);
    }
}
