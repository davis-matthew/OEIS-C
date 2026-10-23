/*
 * A131900  A131821 + A128174 - A000012.
 * https://oeis.org/A131900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry a(i,j) of an infinite lower‑triangular matrix
 *            defined by • if i is even: a(i,0) = a(i,i) = i+1 for 1≤j≤i-1:
 *            a(i,j) = 1 if j is even = 0 if j is odd if i is odd: a(i,0) = i
 *            a(i,i) = i+1 for 1≤j≤i-1: a(i,j) = 1 if j is odd = 0 if j is
 *            even a(n) is the n‑th term in row‑major order...
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t row_from_index(idx_t n)
{
    /* Find the unique i such that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2  */
    idx_t low = 0, high = 1;
    while ( (__int128)high * (high + 1) / 2 <= n ) high <<= 1;
    low = high >> 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ( (__int128)mid * (mid + 1) / 2 <= n ) low = mid;
        else high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t i = row_from_index(n);
    idx_t off = (idx_t)(((seq_t)i * (i + 1)) / 2);
    idx_t j = n - off;           /* column index in row i */

    if (j == 0) {
        return (seq_t)( (i & 1ULL) ? i : (i + 1) );   /* odd→i, even→i+1 */
    }
    if (j == i) {
        return (seq_t)(i + 1);
    }

    if (i & 1ULL) {   /* odd row */
        return (seq_t)( (j & 1ULL) ? 1 : 0 );   /* odd j →1, even j →0 */
    } else {          /* even row */
        return (seq_t)( (j & 1ULL) ? 0 : 1 );   /* odd j →0, even j →1 */
    }
}
