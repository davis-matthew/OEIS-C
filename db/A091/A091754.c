/*
 * A091754  Fifth column (m=6) of array A091752 ((-1,2)Stirling2).
 * https://oeis.org/A091754
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((3*(n-2))! / ((3^(n-2))*(n-2)!)) * (9*n^2-27*n+12) / 4!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3. For smaller n we return 0. */
    if (n < 3)
        return 0;

    idx_t k = n - 2;            /* k = n-2 >= 1 */
    seq_t prod = 1;            /* product ∏_{i=1..k} (3i-2)(3i-1) */

    for (idx_t i = 1; i <= k; ++i) {
        prod *= (seq_t)(3 * i - 2);
        prod *= (seq_t)(3 * i - 1);
    }

    /* Q = 9*n^2 - 27*n + 12, all computed in seq_t to avoid overflow */
    seq_t q = (seq_t)9 * (seq_t)n * (seq_t)n
            - (seq_t)27 * (seq_t)n
            + (seq_t)12;

    prod *= q;

    /* Divide by 4! = 24; the division is exact. */
    prod /= (seq_t)24;

    return prod;
}
