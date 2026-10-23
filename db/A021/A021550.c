/*
 * A021550  Decimal expansion of 1/546.
 * https://oeis.org/A021550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = 0 for n < 2 a(n) = {1,8,3,1,5,0}[(n-2) mod 6] for n ≥ 2
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pat[6] = {
        (seq_t)1,
        (seq_t)8,
        (seq_t)3,
        (seq_t)1,
        (seq_t)5,
        (seq_t)0
    };
    if (n < 2)
        return (seq_t)0;
    size_t idx = (size_t)((n - 2) % 6);
    return pat[idx];
}
