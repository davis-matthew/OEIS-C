/*
 * A018262  Divisors of 50.
 * https://oeis.org/A018262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   all positive divisors of 50 sorted ascending
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   // for size_t
#include <stdint.h>   // for uint64_t
#include <stdbool.h>  // for bool

seq_t a(idx_t n)
{
    static seq_t divs[6];
    static size_t div_count = 0;

    /* Compute the divisor list once */
    if (div_count == 0) {
        size_t i = 0;
        for (uint64_t d = 1; d <= 50ULL; ++d) {
            if (50ULL % d == 0) {
                divs[i++] = (seq_t)d;
            }
        }
        div_count = i;
    }

    if (n == 0 || n > (idx_t)div_count)
        return 0;

    return divs[(size_t)(n - 1)];
}
