/*
 * A204171  Symmetric matrix based on f(i,j)=(1 if max(i,j) is odd, and 0
 *          otherwise), by antidiagonals.
 * https://oeis.org/A204171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if max(i,j) is odd, where (i,j) is the cell reached by
 *            enumerating the infinite matrix f(i,j) = [max(i,j) odd] along
 *            antidiagonals (n = 1 gives (1,1), n = 2 gives (1,2), n = 3 gives
 *            (2,1), …)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the smallest diagonal number d such that d*(d+1)/2 >= n.
   Uses only integer arithmetic. */
static inline idx_t diag_of(idx_t n)
{
    /* Start with a high estimate; double until it is large enough. */
    idx_t high = 1;
    while ((__int128)high * (high + 1) / 2 < (__int128)n)
        high <<= 1;          /* high *= 2 */

    /* Binary search between high/2 and high. */
    idx_t low = high >> 1;    /* low = high/2 */
    while (low < high)
    {
        idx_t mid = (low + high) >> 1;
        __int128 prod = (__int128)mid * (mid + 1) / 2;
        if (prod >= (__int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, return 0 */

    /* Determine which antidiagonal contains the nth entry. */
    idx_t d = diag_of(n);

    /* Number of entries before this diagonal. */
    idx_t prev = ((d - 1) * d) / 2;

    /* Position of the entry within its diagonal (1‑based). */
    idx_t k = n - prev;

    /* Coordinates on the diagonal: (i,j) = (k, d-k+1). */
    idx_t i = k;
    idx_t j = d - k + 1;

    /* Return 1 if max(i,j) is odd, otherwise 0. */
    idx_t m = (i > j) ? i : j;
    return (m & 1) ? (seq_t)1 : (seq_t)0;
}
