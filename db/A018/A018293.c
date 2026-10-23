/*
 * A018293  Divisors of 120.
 * https://oeis.org/A018293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 120 (divisors listed in increasing
 *            order)
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence contains all positive divisors of 120: */
                                        /* 1,2,3,4,5,6,8,10,12,15,20,24,30,40,60,120 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static array for the 16 divisors of 120 */
    static seq_t divs[16];
    static bool init = false;

    if (!init) {
        /* compute the divisors of 120 once */
        size_t k = 0;
        /* 120 fits in 64‑bit, iterate 1..120 */
        for (uint64_t i = 1; i <= 120; ++i) {
            if (120 % i == 0) {
                divs[k++] = (seq_t)i;
            }
        }
        init = true;
    }

    /* n is 1‑based; return 0 for indices outside the range */
    if (n == 0 || n > 16) {
        return (seq_t)0;
    }
    return divs[(size_t)(n - 1)];
}
