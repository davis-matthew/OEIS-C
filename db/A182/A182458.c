/*
 * A182458  a(0)=1, a(1)=2, a(n) = (a(n-2)*a(n-1)+1) mod n.
 * https://oeis.org/A182458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2, a(n) = (a(n‑2) * a(n‑1) + 1) mod n for n >=
 *            2
 * Range:     u8 n <= 30, u16 n <= 680, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;

    seq_t prevprev = 1;   /* a(0) */
    seq_t prev     = 2;   /* a(1) */
    seq_t cur;

    for (idx_t k = 2; k <= n; ++k)
    {
        /* compute a(k) = (a(k-2) * a(k-1) + 1) mod k */
        seq_t x = ((prevprev % (seq_t)k) * (prev % (seq_t)k));
        x = (x + 1) % (seq_t)k;
        cur = x;
        prevprev = prev;
        prev = cur;
    }

    return prev;
}
