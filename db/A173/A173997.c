/*
 * A173997  Irregular triangle by columns derived from (1, 2, 3, ...) * (1, 2,
 *          3, ...).
 * https://oeis.org/A173997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is obtained by reading the irregular triangle defined by
 *            T(r,k) = k * (2 - 2*k + r) row‑wise. For n ≥ 1, let r be the row
 *            containing the n‑th element and k the column (1‑based) within
 *            that row. Then a(n) = T(r,k)
 * Range:     u8 n <= 493, u16 n <= 9922, u32 n <= 9922, u64 n <= 9922,
 *            u128 n <= 9922, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root (floor) for 64‑bit unsigned integers. */
static inline idx_t floor_sqrt(idx_t x)
{
    idx_t low = 0, high = (idx_t)1ULL << 32;          /* 2^32 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {               /* avoid division by zero */
            low = mid + 1;
            continue;
        }
        if (mid <= x / mid) {         /* mid*mid ≤ x */
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;                       /* high is ⌊√x⌋ */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1 – unused case */

    /* Find the block (m) that contains n:
       rows 2m‑2 and 2m‑1 share the same value of m. */
    idx_t m = floor_sqrt(n);

    idx_t row;
    if (n > m * m + m) {               /* beyond the even row of this block */
        ++m;
        row = 2 * m - 1;                /* odd row */
    } else if (n > m * m) {            /* in the even row of this block */
        row = 2 * m;
    } else {                           /* first row of the block (odd row) */
        row = 2 * m - 1;
    }

    /* Count of elements before this row */
    idx_t prev_count;
    if ((row & 1) == 0) {              /* even row: S(r‑1) = m*m */
        prev_count = m * m;
    } else {                           /* odd row: S(r‑1) = (m‑1)*m */
        prev_count = (m - 1) * m;
    }

    /* Column index within the row */
    idx_t k = n - prev_count;

    /* Compute T(r,k) = k * (2 - 2*k + r) */
    seq_t term = (seq_t)k * ( (seq_t)2 - (seq_t)2 * k + (seq_t)row );

    return term;
}
