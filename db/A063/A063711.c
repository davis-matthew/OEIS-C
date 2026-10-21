/*
 * A063711  Table of bits required for product of n- and k-bit nonnegative
 *          numbers read by antidiagonals.
 * https://oeis.org/A063711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = a(i,j) where (i,j) are the coordinates of n in Cantor's
 *            diagonal enumeration of the table a(i,j). For a given pair
 *            (i,j): a(i,j) = i*j if min(i,j) <= 1 a(i,j) = i + j otherwise
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is read by antidiagonals: (0,0), (0,1),(1,0),
   (0,2),(1,1),(2,0), … */

seq_t a(idx_t n)
{
    /* Find the antidiagonal k that contains the n‑th element.
       The number of elements before diagonal k is k*(k+1)/2.
       The total number up to and including diagonal k is
       (k+1)*(k+2)/2.  We stop when the latter exceeds n. */
    idx_t k = 0;
    while (1) {
        unsigned __int128 cnt = (unsigned __int128)(k + 1) * (k + 2) / 2;
        if (cnt > n) break;
        ++k;
    }

    /* Number of elements strictly before this diagonal. */
    unsigned __int128 before128 = (unsigned __int128)k * (k + 1) / 2;
    idx_t before = (idx_t)before128;

    /* Position of n within the current diagonal. */
    idx_t offset = n - before;          /* 0 ≤ offset ≤ k      */
    idx_t i      = offset;              /* row index            */
    idx_t j      = k - offset;          /* column index         */

    /* Apply the definition of a(i,j). */
    if (i <= 1 || j <= 1) {
        return (seq_t)i * (seq_t)j;
    } else {
        return (seq_t)i + (seq_t)j;
    }
}
