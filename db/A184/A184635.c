/*
 * A184635  a(n) = floor(1/{(n+n^4)^(1/4)}), where {} = fractional part.
 * https://oeis.org/A184635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1/{(n+n^4)^(1/4)}) = 4*n^2 for n>=2, a(1)=5
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)5;
    return (seq_t)4 * (seq_t)n * (seq_t)n;
}
