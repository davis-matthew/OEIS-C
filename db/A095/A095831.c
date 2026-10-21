/*
 * A095831  Triangle read by rows: T(n,k) = (n-k)^2, n>=1, 1<=k<=n.
 * https://oeis.org/A095831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = ((r - (n - r*(r-1)/2)))^2 where r = the smallest integer
 *            such that r*(r+1)/2 >= n
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r that contains position n.  r must satisfy
       r*(r+1)/2 >= n and (r-1)*r/2 < n.  We find r by a simple
       doubling followed by binary search.  All arithmetic that can
       overflow 64 bits is performed in a __int128.  */
    idx_t low = 1, high = 1;

    /* Grow high until it is large enough.  */
    while ( (unsigned __int128)high*(high+1)/2 < n )
        high <<= 1;     /* double high */

    /* Binary search between low and high.  */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( (unsigned __int128)mid*(mid+1)/2 >= n )
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                 /* row index */

    /* Compute the column within the row (1‑based).  */
    idx_t prev = (unsigned __int128)r*(r-1)/2;   /* sum of previous rows */
    idx_t col = n - prev;                       /* position in row r */

    /* Value is (r - col)^2.  Compute in seq_t to avoid overflow.  */
    seq_t diff = (seq_t)(r - col);
    return diff * diff;
}
