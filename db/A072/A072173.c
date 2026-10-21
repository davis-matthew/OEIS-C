/*
 * A072173  a(n) = (2*n+1)*239^(2*n+1).
 * https://oeis.org/A072173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*239^(2*n+1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* multiplier (2*n+1) */
    seq_t mult = (seq_t)(2 * (seq_t)n + 1);

    /* exponent 2*n+1 */
    unsigned long long e = 2ULL * (unsigned long long)n + 1ULL;

    /* binary exponentiation of 239^e */
    seq_t result   = (seq_t)1;
    seq_t current  = (seq_t)239;
    while (e != 0) {
        if (e & 1ULL)
            result *= current;
        e >>= 1ULL;
        if (e)
            current *= current;
    }

    return mult * result;
}
