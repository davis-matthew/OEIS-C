/*
 * A078401  Triangle read by rows: T(n,k) = number of numbers <= k that are
 *          coprime to n, 1 <= k <= n.
 * https://oeis.org/A078401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r, c) * where r is the row such that (r-1)*r/2 < n ≤
 *            r*(r+1)/2, * c = n - (r-1)*r/2, * and T(r, c) is the number of
 *            integers ≤ c that are coprime to r. * T(r, c) is computed by
 *            inclusion–exclusion over the prime factors of r: * T(r, c) =
 *            Σ_{d|r} μ(d)·⌊c/d⌋, * where μ(d) = 0 if d is divis...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   coprime_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t, if needed */
#include "../utils/coprime_count.h"

/* Triangle read by rows: a(n) = T(r, c) as defined above */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; n=0 is undefined but return 0 */

/* Find row r: smallest r with r*(r+1)/2 ≥ n */
    idx_t r = 1;
    idx_t sum = 1;
    while (sum < n) {
        ++r;
        sum += r;
    }

/* Column index c in that row */
    idx_t col = n - (r - 1) * r / 2;

/* Return the coprime count for (r, col) */
    return coprime_count(r, col);
}
