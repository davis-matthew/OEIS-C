/*
 * A037111  Trajectory of 3 under map n->27n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 3, a(n+1) = (a(n) % 2 == 0) ? a(n)/2 : 27*a(n)+1
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 24, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Trajectory of 3 under map n->27*n+1 if n odd, n->n/2 if n even. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;

    seq_t val = (seq_t)3;
    for (idx_t i = 0; i < n; ++i)
    {
        if ((val & (seq_t)1) == 0)      /* even check using bitwise & */
            val >>= 1;                  /* val = val / 2 */
        else
            val = val * (seq_t)27 + (seq_t)1;
    }
    return val;
}
