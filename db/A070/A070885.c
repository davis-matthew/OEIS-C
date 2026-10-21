/*
 * A070885  a(n) = (3/2)*a(n-1) if a(n-1) is even; (3/2)*(a(n-1)+1) if a(n-1)
 *          is odd.
 * https://oeis.org/A070885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = 3 * ceil(a(n-1) / 2) (recurrence
 *            equivalent to a(n) = (3/2)*a(n-1) if a(n-1) even;
 *            (3/2)*(a(n-1)+1) if a(n-1) odd)
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 52, u64 n <= 107,
 *            u128 n <= 216, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    seq_t prev = (seq_t)1;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)  /* compute up to a(n) */
    {
        /* ceil(prev/2) == (prev + 1) / 2 for unsigned types */
        seq_t next = (seq_t)3 * ((prev + (seq_t)1) / (seq_t)2);
        prev = next;
    }
    return prev;
}
