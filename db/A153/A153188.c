/*
 * A153188  Triangle read by rows: T(n,k) = n^k * k!.
 * https://oeis.org/A153188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k), where r = floor((sqrt(1+8*n)-1)/2), k = n -
 *            r*(r+1)/2, and T(r,k) = r^k * k! (triangle read by rows)
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 42, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r that contains the index n.  The row starts at
       offset = r*(r+1)/2.  We increase r until the next row would start
       after n.  Complexity is O(sqrt(n)).                      */
    idx_t r = 0;
    idx_t offset = 0;
    while (offset + (r + 1) <= n) {
        r++;
        offset += r;              /* offset now equals sum_{i=1}^{r} i = r*(r+1)/2 */
    }

    /* Position within the row. */
    idx_t k = n - offset;

    /* Compute r^k * k! in a single loop.
       We multiply by r and by i for each i from 1 to k; after k steps
       we have performed k multiplications by r and k multiplications by i,
       yielding the desired product.                                   */
    seq_t value = 1;
    for (idx_t i = 1; i <= k; ++i) {
        value *= (seq_t)r;   /* multiply by r */
        value *= (seq_t)i;   /* multiply by i (factorial) */
    }
    return value;
}
