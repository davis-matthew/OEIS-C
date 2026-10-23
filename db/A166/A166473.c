/*
 * A166473  a(n) = 2^L(n+1) * 3^L(n)/12, where L(n) is the n-th Lucas number
 *          (A000032(n)).
 * https://oeis.org/A166473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^L(n+1) * 3^L(n) / 12
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (for n > 2): a(n) = 12 * a(n-1) * a(n-2), with a(1)=2, a(2)=36 */

#include <stdint.h>  // for uint64_t, if needed

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)2;
    if (n == (idx_t)2)
        return (seq_t)36;

    seq_t a1 = (seq_t)2;   /* a(n-2) */
    seq_t a2 = (seq_t)36;  /* a(n-1) */

    for (idx_t i = (idx_t)3; i <= n; ++i)
    {
        seq_t next = a2 * a1;  /* a(n-1) * a(n-2) */
        next   *= (seq_t)12;   /* times 12 */
        a1 = a2;               /* shift for next iteration */
        a2 = next;
    }

    return a2;
}
