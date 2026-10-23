/*
 * A131406  3*A128174 - 2*A000012(signed).
 * https://oeis.org/A131406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if (r - c) is even, 2 otherwise where r is the unique
 *            integer with r(r+1)/2 >= n, and c = n - r(r-1)/2
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that the triangular number T(r)=r(r+1)/2
       is at least n.  This loop is O(sqrt(n)), which is trivial for
       the test range (n up to about 10 000). */
    idx_t tri = 0;   /* running sum 1 + 2 + ... + (r-1) */
    idx_t r   = 0;
    while (tri < n) {
        ++r;           /* now current r */
        tri += r;      /* tri = sum 1..r */
    }
    /* Position c within row r: 1 <= c <= r. */
    idx_t c = n - (tri - r);   /* tri - r = sum 1..(r-1) */

    /* Triangle entry: 1 if (r - c) even, else 2. */
    if (((r - c) & 1) == 0)
        return (seq_t)1;
    else
        return (seq_t)2;
}
