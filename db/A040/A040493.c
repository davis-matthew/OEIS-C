/*
 * A040493  Continued fraction for sqrt(516).
 * https://oeis.org/A040493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=22; for n>=1, a(n)=pattern[(n-1)%10] where
 *            pattern=[1,2,1,1,14,1,1,2,1,44]
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)22;
    static const seq_t pat[10] = {
        (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)1,
        (seq_t)14, (seq_t)1, (seq_t)1, (seq_t)2,
        (seq_t)1, (seq_t)44
    };
    size_t pos = (size_t)((n - 1) % (idx_t)10);
    return pat[pos];
}
