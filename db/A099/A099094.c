/*
 * A099094  a(n) = 3*a(n-2) + 3*a(n-3), a(0)=1, a(1)=0, a(2)=3.
 * https://oeis.org/A099094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-2) + 3*a(n-3); a(0)=1, a(1)=0, a(2)=3
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)0;
    if (n == 2u) return (seq_t)3;

    /* maintain the three most recent values:
       a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)0;
    seq_t a2 = (seq_t)3;

    idx_t i;
    seq_t next;
    for (i = 3; i <= n; ++i)
    {
        /* next = 3*a1 + 3*a0; */
        next = ((seq_t)3 * a1) + ((seq_t)3 * a0);
        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
