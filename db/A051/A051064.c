/*
 * A051064  3^a(n) exactly divides 3n. Or, 3-adic valuation of 3n.
 * https://oeis.org/A051064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult, changed
 * Formula:   a(n) = v_3(n) + 1 (the exponent of 3 in n, plus 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is also the 3-adic valuation of 3*n */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    while (n % 3 == 0) {
        count++;
        n /= 3;
    }
    return (seq_t)(count + 1);
}
