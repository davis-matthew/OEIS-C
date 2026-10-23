/*
 * A102040  a(n) = a(n-1) + last digit of a(n-1), starting at 3.
 * https://oeis.org/A102040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = a(n-1) + last_digit(a(n-1)), a(1)=3
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* undefined for n < 1; return 0 for safety */
        return 0;
    }

    seq_t val = 3;                /* a(1) */
    if (n == 1) {
        return val;
    }

    /* Compute iteratively: a(k) = a(k-1) + a(k-1) % 10 */
    for (idx_t i = 1; i < n; ++i) {
        seq_t ld = val % (seq_t)10;
        val += ld;
    }

    return val;
}
