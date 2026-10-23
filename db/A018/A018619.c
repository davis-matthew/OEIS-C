/*
 * A018619  Divisors of 738.
 * https://oeis.org/A018619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 738 in ascending order
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divs[12];
    static idx_t divCount = 0;
    static bool initialized = false;

    if (!initialized) {
        divCount = 0;
        for (idx_t i = 1; i <= 738; ++i) {
            if (738 % i == 0) {
                divs[divCount++] = (seq_t)i;
            }
        }
        initialized = true;
    }

    if (n == 0 || n > divCount) {
        return (seq_t)0;
    }
    return divs[n - 1];
}
