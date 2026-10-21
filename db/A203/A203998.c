/*
 * A203998  Symmetric matrix based on f(i,j)=max{i(j+1)-1,j(i+1)-1}, by
 *          antidiagonals.
 * https://oeis.org/A203998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A203998: a(n) = f(i,j) where (i,j) is the n‑th entry on the
 * antidiagonals of the matrix f(i,j)=max{ i*(j+1)-1 , j*(i+1)-1 }.
 * The entries are enumerated as follows:
 *
 * n = 1      -> (i,j) = (1,1)
 * n = 2      -> (1,2)
 * n = 3      -> (2,1)
 * n = 4      -> (1,3)
 * n = 5      -> (2,2)
 * n = 6      -> (3,1)
 * ...
 *
 * i = p
 * j = t+1-p
 * where t is the minimal integer with t*(t+1)/2 ≥ n
 * and p = n - (t-1)*t/2 (1‑based position inside the diagonal).
 *
 * The function works for any n ≥ 1.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the diagonal index t (≥1) such that
       (t-1)*t/2 < n ≤ t*(t+1)/2. */
    idx_t low = 1;
    idx_t high = n;                          /* high is always valid */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t t = low;

    /* Number of elements before this diagonal.  (t-1)*t/2 fits in 64‑bits. */
    idx_t prev = (t - 1) * t / 2;

    /* Position inside the diagonal (1‑based). */
    idx_t p = n - prev;

    /* Compute matrix indices. */
    idx_t i = p;
    idx_t j = t + 1 - p;

    /* Compute the matrix entry f(i,j).  Use 128‑bit arithmetic to avoid overflow. */
    unsigned __int128 val1 = (unsigned __int128)i * (j + 1) - 1;
    unsigned __int128 val2 = (unsigned __int128)j * (i + 1) - 1;
    unsigned __int128 res128 = val1 > val2 ? val1 : val2;

    return (seq_t)res128;
}
