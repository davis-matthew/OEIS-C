/*
 * A210615  Least semiprime dividing n, or 0 if no semiprime divides n.
 * https://oeis.org/A210615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 245, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the least semiprime divisor of n, or 0 if none. */
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* For n < 4 no proper divisor can be a semiprime. */
    if (n < 4)
        return 0;

    seq_t N = (seq_t)n;

    for (idx_t d = 2; d <= n; ++d) {
        seq_t D = (seq_t)d;
        if (N % D == 0 && count_prime_factors(D) == 2)
            return D;
    }

    /* Should never reach here, but return 0 for safety. */
    return 0;
}
