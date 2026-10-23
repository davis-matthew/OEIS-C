/*
 * A018715  Divisors of 908.
 * https://oeis.org/A018715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 908 in increasing order (1-indexed)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* N is the number whose divisors we list. */
    const idx_t N = 908;

    if (n == 0) {
        return (seq_t)0;          /* Undefined for n = 0, but harmless. */
    }

    idx_t count = 0;
    for (idx_t d = 1; d <= N; ++d) {
        if (N % d == 0) {          /* d is a divisor of 908. */
            ++count;
            if (count == n) {
                return (seq_t)d;   /* Return the n-th divisor. */
            }
        }
    }

    /* n exceeds the number of divisors of 908; return 0. */
    return (seq_t)0;
}
