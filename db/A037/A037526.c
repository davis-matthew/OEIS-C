/*
 * A037526  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,0.
 * https://oeis.org/A037526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = base‐9 number consisting of the first n terms of the
 *            periodic pattern 2,1,0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          // undefined for n < 1 – return 0

    static const seq_t PAT[3] = { (seq_t)2, (seq_t)1, (seq_t)0 };

    seq_t val = (seq_t)0;
    for (idx_t i = 0; i < n; ++i)
    {
        val = val * (seq_t)9 + PAT[i % 3];
    }
    return val;
}
