/*
 * A105739  For n>2, a(n) > 0 is such that a(n-1)^2+4*a(n-2)*a(n) is a minimal
 *          square, a(1)=1,a(2)=6.
 * https://oeis.org/A105739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=6. For n>2, a(n) is the smallest positive integer
 *            such that a(n‑1)^2 + 4*a(n‑2)*a(n) is a perfect square
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for standard integer types (may be unused) */
#include <stddef.h>  /* for size_t (may be unused) */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)6;

    seq_t prevprev = (seq_t)1;   /* a(1) */
    seq_t prev     = (seq_t)6;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t A     = prev;            /* a(i-1)   */
        seq_t B     = prevprev;        /* a(i-2)   */
        seq_t denom = (seq_t)4 * B;    /* 4 * a(i-2) */

        seq_t k = A + (seq_t)1;        /* start at first k > A */
        seq_t a_n;

        while (1) {
            seq_t diff = k * k - A * A;   /* k^2 – A^2 */
            if (diff % denom == (seq_t)0) {
                a_n = diff / denom;       /* candidate a(i) */
                if (a_n > (seq_t)0)
                    break;
            }
            ++k;
        }

        prevprev = prev;
        prev     = a_n;
    }

    return prev; /* a(n) */
}
