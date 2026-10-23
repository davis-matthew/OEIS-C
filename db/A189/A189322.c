/*
 * A189322  Number of nondecreasing arrangements of n+2 numbers in 0..5 with
 *          the last equal to 5 and each after the second equal to the sum of
 *          one or two of the preceding four.
 * https://oeis.org/A189322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 38*n - 147 for n >= 7; explicitly defined for n <= 6
 * Range:     u8 n <= 10, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t _a189322_base[7] = {0, 8, 12, 21, 33, 54, 84};

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n = 0, return 0 */
        return 0ULL;

    if (n <= 6U)                /* first six terms are given explicitly */
        return _a189322_base[(size_t)n];

    /* For n >= 7 the linear empirical formula holds. */
    return (seq_t)n * 38ULL - 147ULL;
}
