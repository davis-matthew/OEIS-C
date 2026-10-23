/*
 * A018403  Divisors of 344.
 * https://oeis.org/A018403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 344 in ascending order
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n is at least 1, but guard */
    idx_t divisors[8];
    size_t count = 0;

    for (idx_t i = 1; i <= 344; ++i) {
        if (344 % i == 0) {
            divisors[count++] = i;
            if (count == 8) break; /* maximum number of divisors */
        }
    }

    if (n <= count) return (seq_t)divisors[n - 1];
    return 0;
}
