/*
 * A129765  Triangle, (1, 1, 2, 2, 2, ...) in every column.
 * https://oeis.org/A129765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if n belongs to the last or second‑to‑last position of its
 * triangular row in the triangle defined by (n-2) 2's followed by two 1's
 * (row 1 contains a single 1); otherwise a(n) = 2.
 * More precisely, let r be the smallest integer with r(r+1)/2 ≥ n.
 * If n == r(r+1)/2 or n == r(r+1)/2 - 1 then a(n) = 1, else a(n) = 2.
 * (a(1) = 1) */
seq_t a(idx_t n) {
    /* guard, though n ≥ 1 by the problem statement */
    if (n == 0) return (seq_t)1;

    /* binary search for r such that r(r+1)/2 ≥ n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* position of n within its row (1‑based) */
    __int128 prev = (__int128)(r - 1) * r / 2;
    idx_t pos = (idx_t)(n - prev);    /* 1 ≤ pos ≤ r */

    if (r == 1)                  /* row 1 contains a single 1 */
        return (seq_t)1;

    if (pos == r || pos == r - 1)   /* the last two entries of the row */
        return (seq_t)1;

    return (seq_t)2;                 /* all other entries are 2 */
}
