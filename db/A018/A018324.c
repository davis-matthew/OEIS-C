/*
 * A018324  Divisors of 186.
 * https://oeis.org/A018324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 186 (sorted ascending). For n > 8, a(n) =
 *            0 as 186 has 8 divisors
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   init_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/init_divisors.h"

seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t divs[8];
    if (!initialized) {
        init_divisors(divs);
        initialized = true;
    }
    if (n < 1 || n > 8) return (seq_t)0;
    return divs[(size_t)(n - 1)];
}
