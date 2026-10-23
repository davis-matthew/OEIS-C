/*
 * A108036  Triangle read by rows: the triangle in A108035 surrounded by a
 *          border of 0's.
 * https://oeis.org/A108036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 0 if n is on the border of the triangle, otherwise a(n) =
 *            F_r where r is the row index of n and F_r is the r‑th Fibonacci
 *            number (F_0=0, F_1=1). The triangle has (r+1) elements in row r,
 *            border zeros at positions 0 and r, and all interior elements
 *            equal to F_r
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r that contains index n:
       the total number of elements up to row r is (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Position of n within its row. */
    idx_t base = r * (r + 1) / 2;
    idx_t c = n - base;      /* 0 ≤ c ≤ r */

    /* Border zeros. */
    if (c == 0 || c == r)
        return 0;

    /* Compute the r‑th Fibonacci number in seq_t. */
    seq_t f0 = 0, f1 = 1;   /* f0 = F_0, f1 = F_1 */
    for (idx_t i = 2; i <= r; ++i) {
        seq_t tmp = f0 + f1; /* F_i = F_{i-1} + F_{i-2} */
        f0 = f1;
        f1 = tmp;
    }
    return f1;   /* f1 now holds F_r */
}
