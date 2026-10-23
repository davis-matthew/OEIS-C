/*
 * A127701  Infinite lower triangular matrix with (1, 2, 3, ...) in the main
 *          diagonal, (1, 1, 1, ...) in the subdiagonal and the rest zeros.
 * https://oeis.org/A127701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n-th entry of the lower triangular matrix whose main diagonal contains 1,2,3,… and subdiagonal contains 1’s. */
/* a(n) = r   if n = r(r−1)/2 + r (main diagonal) */
/* a(n) = 1   if n = r(r−1)/2 + (r−1) (subdiagonal) */
/* otherwise a(n) = 0 */
seq_t a(idx_t n)
{
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;
    idx_t prev = (r - 1) * r / 2;
    idx_t col  = n - prev;
    if (col == r)
        return (seq_t)r;
    if (col == r - 1)
        return (seq_t)1;
    return (seq_t)0;
}
