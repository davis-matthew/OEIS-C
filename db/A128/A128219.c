/*
 * A128219  A000012 * A127701. a(1) = 1, a(2) = 2, a(3) = 2; by rows, n-1
 *          terms of 2, 3, 4, ... followed by "n".
 * https://oeis.org/A128219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (p == r ? r : p + 1) where p = n - r(r - 1)/2 and r is
 *            the unique integer such that r(r - 1)/2 < n <= r(r + 1)/2
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A128219. */
seq_t a(idx_t n) {
    /* Find the row r such that T_{r-1} < n <= T_r,
       where T_r = r(r + 1)/2.  Binary search over [1, n]. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute the starting index of row r (i.e., T_{r-1}). */
    unsigned __int128 triPrev128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t startPrev = (idx_t)triPrev128;   /* fits because startPrev < n */

    /* Position within the row. */
    idx_t p = n - startPrev;   /* 1 <= p <= r */

    /* Compute the value: if p == r, value = r, else value = p + 1. */
    seq_t value;
    if (p == r)
        value = (seq_t)r;
    else
        value = (seq_t)(p + 1);

    return value;
}
