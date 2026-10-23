/*
 * A170754  Expansion of g.f.: (1+x)/(1-34*x).
 * https://oeis.org/A170754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n==0 ? 1 : 35 * 34^(n-1))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Fast exponentiation: compute 34^(n-1) */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)34;
    idx_t exp    = n - 1;

    while (exp > 0) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }

    return (seq_t)35 * result;
}
