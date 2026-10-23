/*
 * A185912  Accumulation array of A185910; by antidiagonals.
 * https://oeis.org/A185912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (row*col/6)*(2*row^2 + 3*row + 3*col - 2), where
 *            (row,col) corresponds to the antidiagonal enumeration of
 *            T(row,col) from A185910
 * Range:     u8 n <= 33, u16 n <= 653, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find the diagonal 's' such that n <= s*(s-1)/2.
       s starts at 1; we increment until the condition holds. */
    idx_t s = 1;
    while ((s * (s - 1)) / 2 < n) {
        ++s;
    }

    /* cumulative entries before this diagonal:
       cum_prev = (s-1)*(s-2)/2. */
    idx_t cum_prev = ((s - 1) * (s - 2)) / 2;
    idx_t pos = n - cum_prev;          /* position within the diagonal (1..s-1) */
    idx_t row = pos;                   /* n in T(n,k) */
    idx_t col = s - pos;               /* k in T(n,k) */

    /* Compute the table entry using the given formula.
       All intermediate products fit comfortably in 64‑bit
       arithmetic for the typical range of n. */
    seq_t rn = (seq_t)row;
    seq_t cn = (seq_t)col;
    seq_t part1 = rn * cn;                         /* n*k          */
    seq_t part2 = (seq_t)(2 * row * row + 3 * row + 3 * col - 2);
    seq_t val = (part1 * part2) / 6;               /* (n*k/6)*(...) */

    return val;
}
