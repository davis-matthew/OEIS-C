/*
 * A123316  Triangle read by rows: T(n,k)=(k+1)*n!/2 (1<=k<=n).
 * https://oeis.org/A123316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k+1)*r! / 2, where r = min{ s ≥ 1 : s(s+1)/2 ≥ n } (row
 *            number) k = n - r(r-1)/2 (column index 1‑based) and 1 ≤ k ≤ r
 * Range:     u8 n <= 13, u16 n <= 30, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find r: smallest s such that s*(s+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Compute k */
    idx_t k = n - (r * (r - 1)) / 2;

    /* Compute factorial of r in seq_t */
    seq_t fact = 1;
    for (idx_t i = 2; i <= r; ++i)
        fact *= (seq_t)i;

    /* Compute result */
    seq_t result = fact * (seq_t)(k + 1) / 2;
    return result;
}
