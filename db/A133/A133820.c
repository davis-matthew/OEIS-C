/*
 * A133820  Triangle whose rows are sequences of increasing cubes: 1; 1,8;
 *          1,8,27; ... .
 * https://oeis.org/A133820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (n - (r-1)*r/2)^3 where r = smallest integer with
 *            r(r+1)/2 >= n
 * Range:     u8 n <= 27, u16 n <= 860, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* Offset is 1; undefined for n<1 */

    /* Find the row r such that (r-1)*r/2 < n <= r*(r+1)/2  */
    idx_t low = 1;
    /* Upper bound for r is roughly sqrt(2*n); add a safety margin */
    idx_t high = (idx_t)(sqrtl(2.0L * (long double)n) + 5.0L);

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t r = low;

    /* Compute j = n - tri(r-1)  */
    unsigned __int128 tri_before = (unsigned __int128)(r - 1) * r / 2;
    idx_t j = n - (idx_t)tri_before;

    seq_t val = (seq_t) j;
    val = val * val * val;              /* cube of j */
    return val;
}
