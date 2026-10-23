/*
 * A193654  Q-residue of the triangle p(n,k)=floor((n+1)/(n+k+2)/2), 0<=k<=n,
 *          where Q is the triangular array (t(i,j)) given by t(i,j)=1. (See
 *          Comments.)
 * https://oeis.org/A193654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 7 a(2) = 28 a(3) = 94 a(4) = 275 a(5) = 765 a(6)
 *            = 2002 For n ≥ 7: a(n) = 5*a(n‑1) - 3*a(n‑2) - 21*a(n‑3) +
 *            36*a(n‑4) - 32*a(n‑6) + 16*a(n‑7) (Derived from the rational
 *            generating function of the sequence.)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* handle initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)28;
    if (n == 3) return (seq_t)94;
    if (n == 4) return (seq_t)275;
    if (n == 5) return (seq_t)765;
    if (n == 6) return (seq_t)2002;

    /* sliding array of the last 7 computed values */
    seq_t buf[7];
    buf[0] = (seq_t)1;   /* a(0) */
    buf[1] = (seq_t)7;   /* a(1) */
    buf[2] = (seq_t)28;  /* a(2) */
    buf[3] = (seq_t)94;  /* a(3) */
    buf[4] = (seq_t)275; /* a(4) */
    buf[5] = (seq_t)765; /* a(5) */
    buf[6] = (seq_t)2002;/* a(6) */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t a1 = buf[(i - 1) % 7];
        seq_t a2 = buf[(i - 2) % 7];
        seq_t a3 = buf[(i - 3) % 7];
        seq_t a4 = buf[(i - 4) % 7];
        seq_t a6 = buf[(i - 6) % 7];
        seq_t a7 = buf[(i - 7) % 7];

        seq_t cur = 5 * a1 - 3 * a2 - 21 * a3 + 36 * a4 - 32 * a6 + 16 * a7;
        buf[i % 7] = cur;
    }

    return buf[n % 7];
}
