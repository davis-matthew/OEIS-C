/*
 * A017907  Expansion of 1/(1 - x^13 - x^14 - ...).
 * https://oeis.org/A017907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1..12)=0, a(n)=a(n-1)+a(n-13) for n≥13 (OEIS A017907 –
 *            number of compositions of n into parts ≥13)
 * Range:     u8 n <= 56, u16 n <= 92, u32 n <= 166, u64 n <= 313,
 *            u128 n <= 606, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* A small circular buffer of the last 13 values. */
    seq_t buf[13];
    for (size_t i = 0; i < 13; ++i)
        buf[i] = 0;
    buf[0] = 1;                 /* a(0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t next = 0;
        if (i >= 13) {
            /* (i-1) and (i-13) are safe for idx_t (unsigned). */
            seq_t prev1  = buf[(size_t)((i - 1) % 13)];
            seq_t prev13 = buf[(size_t)((i - 13) % 13)];
            next = prev1 + prev13;
        }
        buf[(size_t)(i % 13)] = next;   /* store a(i) */
    }

    return buf[(size_t)(n % 13)];
}
