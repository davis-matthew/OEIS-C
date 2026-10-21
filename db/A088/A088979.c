/*
 * A088979  a(n) = n! - ((n-1)!!)^2.
 * https://oeis.org/A088979
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - ((n-1)!!)^2 (offset 2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t used by idx_t */

seq_t a(idx_t n)
{
    /* It is required that n >= 2.  For safety, return 0 for smaller inputs. */
    if (n < 2)
        return 0;

    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Compute double‑factorial (n-1)!! */
    seq_t dbl = 1;
    for (idx_t k = n - 1; k >= 2; k -= 2)
        dbl *= (seq_t)k;

    /* Result: n! - ((n-1)!!)^2 */
    return fact - (dbl * dbl);
}
