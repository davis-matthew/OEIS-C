/*
 * A066068  a(n) = n^n + n.
 * https://oeis.org/A066068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n + n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes a(n) = n^n + n.
 * Uses exponentiation by squaring with seq_t arithmetic.
 * Works for all supported seq_t types, including arbitrary-precision.
 */
seq_t a(idx_t n)
{
    seq_t result = 1;          /* holds the power n^n */
    seq_t base   = (seq_t)n;   /* base = n */
    idx_t  exp   = n;          /* exponent = n */

    /* binary exponentiation: O(log n) multiplications */
    while (exp > 0) {
        if (exp & 1ULL)
            result = result * base;
        base = base * base;
        exp >>= 1ULL;
    }

    /* add n */
    result += (seq_t)n;
    return result;
}
