/*
 * A090971  Sierpiński's triangle, read by rows, starting from 1: T(n,k) =
 *          (T(n-1,k) + T(n-1,k-1)) mod 2.
 * https://oeis.org/A090971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if (r & c) == c, else 0 where r = the row number of the
 *            flattened Sierpiński triangle for the n‑th element, and c =
 *            position of the element inside that row (both 1‑based). The row
 *            number r is the smallest integer satisfying r*(r+1)/2 >= n. c =
 *            n - (r-1)*r/2
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find row r such that total elements up to row r is at least n */
    idx_t r = 1;
    while ( ((unsigned __int128)r * (r + 1)) / 2 < n )
        ++r;

    /* Position of n within its row (1‑based) */
    idx_t c = n - ((unsigned __int128)(r - 1) * r) / 2;

    /* Sierpiński triangle entry: 1 if (r & c) == c else 0 */
    return (((r & c) == c) ? (seq_t)1 : (seq_t)0);
}
