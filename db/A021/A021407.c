/*
 * A021407  Decimal expansion of 1/403.
 * https://oeis.org/A021407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = ⌊10 * r_{n-1} / 403⌋ with r_0 = 1, r_k = (10 * r_{k-1})
 *            mod 403. This generates the decimal digits of 1/403, starting
 *            with a(0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The remainder after each division step; it always stays < 403. */
    uint64_t rem = 1;
    seq_t digit;

    /* Iterate from i = 0 up to n, computing each digit in turn. */
    for (idx_t i = 0; i <= n; ++i) {
        /* Compute the digit: integer division truncates towards zero. */
        digit = (seq_t)((10ULL * rem) / 403ULL);

        if (i == n) {
            return digit;
        }

        /* Update the remainder for the next step. */
        rem = (10ULL * rem) % 403ULL;
    }

    /* Should never reach here. */
    return (seq_t)0;
}
