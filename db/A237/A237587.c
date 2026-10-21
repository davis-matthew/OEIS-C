/*
 * A237587  Triangle read by rows in which row n lists the first n odd squares
 *          in decreasing order.
 * https://oeis.org/A237587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (2*(n-k+1)-1)^2 for row n and column k. Sequence a(n)
 *            is read by rows: a(1)=1, a(2)=9, a(3)=1, …
 * Range:     u8 n <= 36, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the row r such that S_{r-1} < n <= S_r,
       where S_r = r*(r+1)/2.  Binary search with 128-bit
       intermediate arithmetic to avoid overflow.             */
    idx_t low = 1;
    idx_t high = 0xFFFFFFFFu;          /* upper bound: sqrt(2*2^64) < 2^32  */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Number of terms before row r */
    unsigned __int128 prev_sum = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = (idx_t)(n - prev_sum);   /* 1‑based position in row r */

    /* Compute the odd number that appears at column k in row r:
       odd = 2*(r-k+1) - 1                                         */
    seq_t odd = (seq_t)(2 * (seq_t)(r - k + 1) - 1);

    /* Return the square of that odd number.  All arithmetic is
       performed in seq_t to preserve exactness on arbitrary‑precision builds. */
    return odd * odd;
}
