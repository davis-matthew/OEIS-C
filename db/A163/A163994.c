/*
 * A163994  Triangle read by rows in which row n contains the first column of
 *          the n-th triangle of A164002.
 * https://oeis.org/A163994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + min( min(i-1, r-i), floor(r/3) ) where r = ceil(
 *            (sqrt(8n+1)-1)/2 ) is the row containing n, and i = n -
 *            (r-1)*r/2 is the position within that row (1‑based)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A163994 */
seq_t a(idx_t n)
{
    /* Find the row r such that tri(r) = r*(r+1)/2 >= n */
    idx_t r = 1;
    idx_t tri = 1;                  /* tri(r) for r=1 */
    while (tri < n) {
        ++r;
        tri += r;                   /* tri(r) = tri(r-1) + r */
    }

    /* tri_prev = tri(r-1) = tri - r */
    idx_t tri_prev = tri - r;
    idx_t i = n - tri_prev;         /* 1‑based position in the row */

    idx_t left  = i - 1;            /* steps from the left end */
    idx_t right = r - i;            /* steps from the right end */
    idx_t min_lr = left < right ? left : right;

    idx_t lim = r / 3;              /* floor(r/3) */
    idx_t offset = min_lr < lim ? min_lr : lim;

    return (seq_t)(offset + 1);
}
