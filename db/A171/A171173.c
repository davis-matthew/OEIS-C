/*
 * A171173  Triangle read by rows in which row n lists A033627(n) together
 *          with the first 2n-1 positive integers.
 * https://oeis.org/A171173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = first value of row r(A033627(r)) if n is the * first
 *            element in its row, otherwise a(n) = (offset within row)-1. *
 *            A033627(r) is 2 for r = 1, and 3*r – 2 for r > 1
 * Range:     u8 n <= 7310, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* defined only for n>=1 */

    /* Find the row r such that r(r+1) >= n and (r-1)r < n. */
    idx_t low = 1;
    idx_t high = (idx_t)(sqrt((long double)n)) + 2; /* upper bound */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Number of terms in all previous rows: (r-1) * r. */
    unsigned __int128 prev_terms = (unsigned __int128)(r - 1) * r;
    idx_t offset = (idx_t)(n - prev_terms);    /* 1 .. 2*r */

    if (offset == 1) {                         /* special term A033627(r) */
        if (r == 1)
            return (seq_t)2;
        else
            return (seq_t)(3 * (seq_t)r - 2);
    } else {
        return (seq_t)(offset - 1);            /* 1 .. 2*r-1 */
    }
}
