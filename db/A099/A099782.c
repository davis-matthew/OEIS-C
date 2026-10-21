/*
 * A099782  a(n) = Sum_{k=0..floor(n/3)} C(n-k,2*k) * 2^k * 4^(n-3*k).
 * https://oeis.org/A099782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 16*a(n-2) + 2*a(n-3) with a(0)=1, a(1)=4,
 *            a(2)=16
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)16;

    /* Use recurrence.  No memoisation – compute from scratch each call. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)4;   /* a(1) */
    seq_t a2 = (seq_t)16;  /* a(2) */
    seq_t ak = a2;

    for (idx_t i = 3; i <= n; ++i) {
        /* ak = 8*a2 - 16*a1 + 2*a0 */
        ak = (seq_t)((seq_t)8 * a2 - (seq_t)16 * a1 + (seq_t)2 * a0);
        a0 = a1;
        a1 = a2;
        a2 = ak;
    }
    return ak;
}
