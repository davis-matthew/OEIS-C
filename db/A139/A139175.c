/*
 * A139175  a(n) = (n! - 5)/5.
 * https://oeis.org/A139175
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! - 5) / 5 = (n! / 5) - 1, for n >= 5
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes a(n) for any unsigned integer type seq_t and idx_t.
 * It multiplies the factors 2 … n, divides by 5 exactly once when the
 * first factor of 5 is encountered, and finally subtracts 1.
 * This keeps the intermediate product within the range of seq_t as long
 * as the final result (a(n)) fits in seq_t, which is guaranteed for all
 * terms that the tests consider. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;                      /* below the offset – undefined */

    seq_t prod = 1;
    bool divided = false;

    for (idx_t i = 2; i <= n; ++i)
    {
        prod *= (seq_t)i;               /* multiply by i */

        /* divide by 5 only once, when possible */
        if (!divided && (prod % 5 == 0))
        {
            prod /= 5;
            divided = true;
        }
    }

    /* a(n) = (n! / 5) - 1 */
    return prod - 1;
}
