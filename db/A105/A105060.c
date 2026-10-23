/*
 * A105060  Triangle read by rows in which the n-th row consists of the first
 *          n nonzero terms of A033312.
 * https://oeis.org/A105060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the triangle read by rows where the n-th row consists of
 *            the first n non‑zero terms of A033312. For a given index n
 *            (offset 1) we find the row r and column c by the standard
 *            triangular array convention, then return T(r,c) = 1 + Σ_{i=2..c}
 *            i · i! where T(r,1)=1. The value is computed direc...
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 77, u64 n <= 209,
 *            u128 n <= 594, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t compute_T(idx_t row, idx_t col)
{
    /* T(row,1)=1 */
    seq_t sum  = 1;
    if (col == 1)
        return sum;
    /* factorial = i! */
    seq_t fact = 1;     /* 1! = 1 */
    for (idx_t i = 2; i <= col; ++i) {
        fact = fact * i;          /* fact = i!   */
        sum  = sum + i * fact;    /* add i·i!   */
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 1)          /* below the offset – not used in tests */
        return 0;

    /* Find the row r such that r*(r+1)/2 >= n.  A simple linear search is
       fast enough for n up to many thousands. */
    idx_t r = 1;
    idx_t tri = 1;       /* r*(r+1)/2 when r=1 */
    while (tri < n) {
        ++r;
        tri = r * (r + 1) / 2;
    }

    /* Column c is the position within the row: */
    idx_t prev = (r - 1) * r / 2;   /* rows before r */
    idx_t c = n - prev;

    return compute_T(r, c);
}
