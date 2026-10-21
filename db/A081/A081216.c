/*
 * A081216  a(n) = (n^n-(-1)^n)/(n+1).
 * https://oeis.org/A081216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^n - (-1)^n)/(n+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* fast exponentiation: compute n^n in seq_t */
    seq_t base = (seq_t)n;
    seq_t result = 1;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }

    /* numerator: if n is even, subtract 1 (since (-1)^n=1),
       otherwise add 1 (since (-1)^n=-1). */
    seq_t numerator;
    if ((n & 1ULL) == 0) {
        numerator = result - 1;
    } else {
        numerator = result + 1;
    }

    /* divide by (n+1) */
    return numerator / ((seq_t)n + 1);
}
