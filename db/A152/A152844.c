/*
 * A152844  Triangle read by rows, A007318 rows repeated six times .
 * https://oeis.org/A152844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = binomial(r, k) where r is the unique non‑negative integer
 *            * satisfying 3*r*(r+1) ≤ n < 3*(r+1)*(r+2), and * k = (n -
 *            3*r*(r+1)) mod (r+1). * This produces the triangle of binomial
 *            coefficients (A007318) * with each row repeated six times
 * Range:     u8 n <= 398, u16 n <= 1017, u32 n <= 3795, u64 n <= 7649,
 *            u128 n <= 7649, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* All values for n < 6 belong to row 0. */
    if (n < 6)
        return (seq_t)1;

    /* Find the row r by exponential search followed by binary search. */
    idx_t low = 0, high = 1;

    /* Exponential search to locate an upper bound for r. */
    while (true) {
        unsigned __int128 tot = (__int128)3 * (high + 1) * (high + 2);
        if (tot > n)
            break;
        low = high;
        /* Guard against wrap‑around when doubling. */
        if (high > ((idx_t)-1) / 2) {
            high = (idx_t)-1;
            break;
        }
        high <<= 1;
    }

    /* Binary search between low and high to find the minimal r
     * with 3*(r+1)*(r+2) > n. After the loop low+1 == high. */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tot = (__int128)3 * (mid + 1) * (mid + 2);
        if (tot > n)
            high = mid;
        else
            low = mid;
    }

    idx_t r = high;  /* row index */

    /* Total elements before row r. */
    unsigned __int128 before128 = (__int128)3 * r * (r + 1);
    idx_t before = (idx_t)before128;

    /* Position within the repeated block of row r. */
    idx_t offset = n - before;
    idx_t k = offset % (r + 1);

    /* Compute binomial coefficient C(r, k) multiplicatively. */
    if (k > r - k)
        k = r - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result *= (seq_t)(r - k + i);
        result /= (seq_t)i;
    }

    return result;
}
