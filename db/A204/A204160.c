/*
 * A204160  Symmetric matrix based on f(i,j)=(3i-2 if i=j and = 0 otherwise),
 *          by antidiagonals.
 * https://oeis.org/A204160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1, except for indices n that correspond to a diagonal entry of
 * the matrix, i.e., n = (s-1)*s/2 + s/2 with s even, where the value is
 * 3*(s/2)-2.  Here s is the antidiagonal index. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find k such that k*(k+1)/2 >= n, where k = s-1 is the
     * number of antidiagonals preceding the one that contains the
     * nth entry. */
    idx_t low = 0;
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;               /* double until the bound holds */
    }
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n) {
            high = mid;
        } else {
            low = mid;
        }
    }
    idx_t k = high;          /* k = s-1 */
    idx_t s = k + 1;         /* antidiagonal index */
    idx_t prev = (unsigned __int128)(k - 1) * k / 2;   /* sum up to previous antidiagonal */
    idx_t offset = n - prev - 1;                       /* 0‑based position inside the antidiagonal */
    idx_t i = offset + 1;                              /* row index of the entry */

    /* Entry is the main diagonal iff s is even and i equals s/2. */
    if ((s & 1) == 0 && i == (s >> 1)) {
        return (seq_t)3 * (seq_t)i - (seq_t)2;
    }
    return (seq_t)1;
}
