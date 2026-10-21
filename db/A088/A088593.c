/*
 * A088593  a(0)=18; a(n+1) = largest odd factor of 3*a(n)+1; stop when
 *          a(n)=1.
 * https://oeis.org/A088593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, fini, full
 * Formula:   a(0) = 18; a(n+1) = largest odd factor of (3 * a(n) + 1)
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cur = 18;                      /* a(0) */
    if (n == 0)
        return cur;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t t = cur * 3 + 1;           /* compute 3*a + 1   */
        /* divide out all factors of 2 (i.e., keep the largest odd factor) */
        while ((t & 1) == 0)
            t >>= 1;
        cur = t;
        if (cur == 1)                    /* sequence stabilises at 1 */
            break;
    }
    return cur;
}
