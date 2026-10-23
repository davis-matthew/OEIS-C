/*
 * A040512  Continued fraction for sqrt(536).
 * https://oeis.org/A040512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=23; for n>0, a(n)=pattern[(n-1) mod 10] with
 *            pattern={6,1,1,2,5,2,1,1,6,46}
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)23;
    static const seq_t pattern[10] = {
        (seq_t)6, (seq_t)1, (seq_t)1, (seq_t)2,
        (seq_t)5, (seq_t)2, (seq_t)1, (seq_t)1,
        (seq_t)6, (seq_t)46
    };
    return pattern[(size_t)((n - 1) % 10)];
}
