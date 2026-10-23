/*
 * A157454  Triangle read by rows: T(n, m) = min(2*m - 1, 2*(n - m) + 1).
 * https://oeis.org/A157454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = min( 2*k-1, 2*(r-k)+1 ) where r is the smallest integer
 *            with r*(r+1)/2 ≥ n and k = n - r*(r-1)/2 (the column index in
 *            row r)
 * Range:     u8 n <= 7875, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r and the column k for the index n.
       r is the smallest integer with r*(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t remaining = n;
    while (remaining > r) {
        remaining -= r;
        ++r;
    }
    idx_t k = remaining;           /* column index (1‑based) */

    /* Compute the two candidate odd numbers and return the smaller. */
    seq_t small = (seq_t)2 * (seq_t)k - 1;          /* 2*k-1      */
    seq_t large = (seq_t)2 * ((seq_t)(r - k)) + 1;  /* 2*(r-k)+1  */
    return small <= large ? small : large;
}
