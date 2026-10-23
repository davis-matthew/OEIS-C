/*
 * A040234  Continued fraction for sqrt(250).
 * https://oeis.org/A040234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=15; for n>=1, a(n) repeats the pattern 1,4,3,3,4,1,30
 *            cyclically
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t cycle[7] = {
        (seq_t)1, (seq_t)4, (seq_t)3, (seq_t)3,
        (seq_t)4, (seq_t)1, (seq_t)30
    };

    if (n == 0)
        return (seq_t)15;
    /* Compute (n-1) modulo 7 safely using idx_t */
    return cycle[(size_t)((n - 1) % 7)];
}
