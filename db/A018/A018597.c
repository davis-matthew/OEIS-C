/*
 * A018597  Divisors of 696.
 * https://oeis.org/A018597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 696 in ascending order
 * Range:     u8 n <= 14, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static seq_t divs[16];
static size_t dcount = 0;

seq_t a(idx_t n)
{
    if (dcount == 0) {
        /* compute all positive divisors of 696 in ascending order */
        const idx_t target = 696;
        for (idx_t k = 1; k <= target; ++k) {
            if ((target % k) == 0) {
                divs[dcount++] = (seq_t)k;
            }
        }
    }
    if (n == 0 || n > dcount) return 0;
    return divs[n - 1];
}
