/*
 * A128621  A127648 * A128174 as an infinite lower triangular matrix.
 * https://oeis.org/A128621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the nth entry of the infinite lower triangular matrix
 *            whose rows are numbered n and entries are n if (n+k) is even,
 *            otherwise 0. For a given n we find the unique row r such that
 *            r*(r-1)/2 < n ≤ r*(r+1)/2, let c = n - r*(r-1)/2 (the column
 *            index, 1‑based). Then a(n) = r if (r+c) is even, e...
 * Range:     u8 n <= 91, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, so n=0 is undefined, return 0 for safety */
        return (seq_t)0;

    idx_t r = 1;
    seq_t t = (seq_t)r * (r + 1) / 2;   /* triangular number t_r */

    while (t < n) {
        ++r;
        t = (seq_t)r * (r + 1) / 2;
    }

    idx_t prev = r * (r - 1) / 2;   /* t_{r-1} */
    idx_t c = n - prev;             /* column in row r */

    /* return r if (r+c) is even, otherwise 0 */
    return (((r + c) & 1) == 0) ? (seq_t)r : (seq_t)0;
}
