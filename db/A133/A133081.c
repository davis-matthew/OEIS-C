/*
 * A133081  An interpolation operator, companion to A133080.
 * https://oeis.org/A133081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 1 if n lies on the subdiagonal (j = i-1) or on the main
 *            diagonal with i odd; otherwise a(n) = 0. Here the sequence is
 *            the row‑major flattening of the infinite lower‑triangular matrix
 *            whose main diagonal is (1,0,1,0,…) and whose subdiagonal is
 *            (1,1,1,…). This follows from the definition of...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed */

/* Find the row index i such that T(i-1) < n ≤ T(i),
   where T(k) = k(k+1)/2 is the k‑th triangular number. */
static idx_t
find_row(idx_t n)
{
    idx_t low = 1, high = 1;
    /* Expand high until it covers n */
    while ( ( (__int128)high * (high + 1) ) / 2 < n )
        high <<= 1;

    /* Binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( ( (__int128)mid * (mid + 1) ) / 2 >= n )
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* n is 1‑based */
    if (n == 0) return 0;   /* should not happen for valid sequences */

    /* Determine row i and column j (1‑based) */
    idx_t i = find_row(n);
    idx_t start = ( (__int128)(i - 1) * i ) / 2;   /* first index of row i */
    idx_t j = n - start;                           /* column index of the element */

    /* Subdiagonal (j = i-1) is always 1 for i ≥ 2 */
    if (j == i - 1) return (seq_t)1;

    /* Main diagonal: 1 if i is odd, 0 if even */
    if (j == i) return (seq_t)((i & 1) ? 1 : 0);

    /* All other positions are 0 */
    return (seq_t)0;
}
