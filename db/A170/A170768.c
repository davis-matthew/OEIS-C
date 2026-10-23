/*
 * A170768  Expansion of g.f.: (1+x)/(1-48*x).
 * https://oeis.org/A170768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n = 0) Formula: a(n) = 49 * 48^(n-1) (n > 0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* compute 48^(n-1) */
    seq_t power = (seq_t)1;
    idx_t m = n - 1;
    while (m > 0) {
        power = power * (seq_t)48;
        m--;
    }
    /* multiply by 49 */
    return power * (seq_t)49;
}
