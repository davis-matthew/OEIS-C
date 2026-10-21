/*
 * A234255  Decimal expansion of -B(12) = 691/2730, 13th Bernoulli number
 *          without sign.
 * https://oeis.org/A234255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = 0 for n = 1, * 2 for n = 2, * 5,3,1,1,3,5 repeating for n
 *            ≥ 3 (period 6 starting at n = 3)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1ULL)
        return (seq_t)0;
    if (n == 2ULL)
        return (seq_t)2;

    /* Periodic part: [5,3,1,1,3,5] repeats from n = 3 onward. */
    static const seq_t period[6] = {5, 3, 1, 1, 3, 5};

    /* (n-3) % 6 yields indices 0..5 for n ≥ 3. */
    idx_t idx = (n - 3ULL) % 6ULL;
    return period[(size_t)idx];
}
