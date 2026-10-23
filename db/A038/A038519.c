/*
 * A038519  Number of elements of GF(2^n) with trace 0 and subtrace 1.
 * https://oeis.org/A038519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-2) + 4*a(n-3) for n > 3 Initial values: a(0)=1,
 *            a(1)=0, a(2)=1, a(3)=3 This implements the recurrence supplied
 *            in the OEIS entry
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uintptr_t if needed */

seq_t a(idx_t n)
{
    /* Buffer to hold the last four computed terms.
       Index i modulo 4 corresponds to a(i). */
    seq_t buf[4];

    buf[0] = (seq_t)1;  /* a(0) */
    buf[1] = (seq_t)0;  /* a(1) */
    buf[2] = (seq_t)1;  /* a(2) */
    buf[3] = (seq_t)3;  /* a(3) */

    /* Compute terms for n >= 4 */
    for (idx_t i = 4; i <= n; ++i)
    {
        /* i-2 and i-3 modulo 4 give the required previous terms */
        seq_t v = (seq_t)2 * buf[(i - 2) & 3] + (seq_t)4 * buf[(i - 3) & 3];
        buf[i & 3] = v;
    }

    return buf[n & 3];
}
