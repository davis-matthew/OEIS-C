/*
 * A162610  Triangle read by rows in which row n lists n terms, starting with
 *          2n-1, with gaps = n-1 between successive terms.
 * https://oeis.org/A162610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   a(n) = (k+1)*(r-1)+1, where r = ceil((sqrt(8*n+1)-1)/2) and * k
 *            = n - r*(r-1)/2. Equivalently, a(n) is the (n‑th) entry of the *
 *            triangle T(n,k) = (k+1)*(n-1)+1 read row by row
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; n=0 is undefined */

    idx_t r   = 1;                 /* current row number   */
    idx_t rem = n;                 /* remaining index in rows */

    /* Find the row r containing the n-th term.
       After the loop rem will be the position k within that row. */
    while (rem > r) {
        rem -= r;
        ++r;
    }

    idx_t k = rem;                 /* 1 ≤ k ≤ r */
    return (seq_t)(k + 1) * (seq_t)(r - 1) + 1;
}
