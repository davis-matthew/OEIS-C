/*
 * A018362  Divisors of 264.
 * https://oeis.org/A018362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 264 in ascending order
 * Range:     u8 n <= 15, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Precompute the 16 divisors of 264 once. */
    static seq_t divs[16];
    static size_t div_cnt = 0;

    if (div_cnt == 0) {
        size_t i = 0;
        for (idx_t d = 1; d <= (idx_t)264; ++d) {
            if (264 % d == 0) {
                divs[i++] = (seq_t)d;
            }
        }
        div_cnt = i; /* should be 16 for 264 */
    }

    if (n == 0 || (n - 1) >= div_cnt)
        return (seq_t)0;   /* undefined for n > number of divisors */

    return divs[n - 1];
}
