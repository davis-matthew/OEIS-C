/*
 * A018481  Divisors of 486.
 * https://oeis.org/A018481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th divisor of 486 in increasing order
 * (a(1)=1,a(2)=2,a(3)=3,a(4)=6,...,a(12)=486).  For n>12 returns 0. */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divs[64];
    static size_t count = 0;
    static bool init = false;

    if (!init) {
        size_t c = 0;
        /* Find all divisors of 486 */
        for (idx_t i = 1; i * i <= 486; ++i) {
            if (486 % i == 0) {
                divs[c++] = (seq_t)i;
                if (i != 486 / i)
                    divs[c++] = (seq_t)(486 / i);
            }
        }
        /* Sort ascending */
        for (size_t p = 0; p < c; ++p) {
            for (size_t q = p + 1; q < c; ++q) {
                if (divs[q] < divs[p]) {
                    seq_t tmp = divs[p];
                    divs[p] = divs[q];
                    divs[q] = tmp;
                }
            }
        }
        count = c;
        init = true;
    }
    if (n == 0 || n > count)
        return (seq_t)0;
    return divs[(size_t)(n - 1)];
}
