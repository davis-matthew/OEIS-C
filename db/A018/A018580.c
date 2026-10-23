/*
 * A018580  Divisors of 664.
 * https://oeis.org/A018580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 664
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t divs[665];
    size_t count = 0;
    for (idx_t i = 1; i <= 664; i++) {
        if (664 % i == 0) {
            divs[count++] = (seq_t)i;
        }
    }
    return (n <= count) ? divs[(size_t)(n - 1)] : 0;
}
