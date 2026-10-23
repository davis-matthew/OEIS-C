/*
 * A172273  a(n) = floor(n*(sqrt(11) - sqrt(2))).
 * https://oeis.org/A172273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * (sqrt(11) - sqrt(2)))
 * Range:     u8 n <= 134, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the special case a(0) */
    if (n == 0) return 0;

    /* Binary search for the largest integer k such that
       k <= n * (sqrt(11) - sqrt(2))
       The inequality k <= n*(sqrt(11) - sqrt(2)) is equivalent to
       44*n^2*k^2 <= (9*n^2 + k^2)^2,
       which contains only integer operations. */
    uint64_t low = 0;
    uint64_t high = 2 * n;        /* Safe upper bound: floor(n*α) < 2n for α<2 */
    uint64_t best = 0;

    while (low <= high) {
        uint64_t mid = (low + high) >> 1;

        /* All intermediate values are kept in 128‑bit to avoid overflow
           for 64‑bit builds.  When seq_t is larger (e.g. arbitrary‑precision)
           the cast to uint64_t is safe because n and mid fit into uint64_t. */
        unsigned __int128 n_sq  = (unsigned __int128)n * n;
        unsigned __int128 k_sq  = (unsigned __int128)mid * mid;
        unsigned __int128 lhs   = 44 * n_sq * k_sq;
        unsigned __int128 rhs   = (unsigned __int128)(9 * n_sq + k_sq);
        rhs = rhs * rhs;

        if (lhs <= rhs) {          /* mid is a valid candidate, try larger */
            best = mid;
            low = mid + 1;
        } else {                   /* mid is too large, try smaller */
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return (seq_t)best;
}
