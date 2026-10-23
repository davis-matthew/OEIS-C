/*
 * A135539  Triangle read by rows: T(n,k) = number of divisors of n that are
 *          >= k.
 * https://oeis.org/A135539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the flattening of the triangle T(n,k) = number of
 *            divisors of n that are >= k; rows are read by rows. a(n) =
 *            T(r,k) where r is the unique integer with r(r+1)/2 ≥ n and k = n
 *            - (r-1)r/2
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the row r such that 1 + 2 + … + r ≥ n */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++r;
        sum += r;
    }
    /* Compute the column index k (1‑based) within the row r */
    idx_t k = n - (sum - r);

    /* Count divisors of r that are ≥ k */
    seq_t cnt = 0;
    idx_t limit = 1;
    while ((limit + 1) * (limit + 1) <= r)
        ++limit;

    for (idx_t d = 1; d <= limit; ++d) {
        if (r % d == 0) {
            if (d >= k) ++cnt;
            idx_t e = r / d;
            if (e != d && e >= k) ++cnt;
        }
    }
    return cnt;
}
