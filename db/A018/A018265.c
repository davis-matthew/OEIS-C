/*
 * A018265  Divisors of 56.
 * https://oeis.org/A018265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute divisors of 56.
   a(n) is the n‑th smallest divisor of 56 (1‑indexed).
   We precompute all divisors once and return the nth value.
   For n > 8 the sequence is undefined; this implementation
   returns 0. */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* cache of the 8 divisors of 56 */
    static seq_t divisors[8];
    static idx_t count = 0;
    static bool init = false;

    if (!init) {
        for (idx_t d = 1; d <= 56; ++d) {
            if ((56 % d) == 0) {
                divisors[count++] = (seq_t)d;
            }
        }
        init = true;
    }

    if (n >= 1 && n <= count) {
        return divisors[n - 1];
    }
    return (seq_t)0;
}
