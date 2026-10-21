/*
 * A212568  Number of (w,x,y,z) with all terms in {1,...,n} and w<|x-y|+|y-z|.
 * https://oeis.org/A212568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 5*a(n-4) - 4*a(n-5) + a(n-6)
 *            Initial values: a(0) = 0, a(1) = 0, a(2) = 2, a(3) = 24, a(4) =
 *            98, a(5) = 272
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)24;
    if (n == 4) return (seq_t)98;
    if (n == 5) return (seq_t)272;

    seq_t buf[6] = {0, 0, 2, 24, 98, 272};

    for (idx_t k = 6; k <= n; ++k)
    {
        int64_t t1 = (int64_t)4 * (int64_t)buf[5];  /* 4*a(k-1) */
        int64_t t2 = (int64_t)5 * (int64_t)buf[4];  /* 5*a(k-2) */
        int64_t t3 = (int64_t)5 * (int64_t)buf[2];  /* 5*a(k-4) */
        int64_t t4 = (int64_t)4 * (int64_t)buf[1];  /* 4*a(k-5) */
        int64_t next_signed = t1 - t2 + t3 - t4 + (int64_t)buf[0];

        seq_t next = (seq_t)next_signed;

        /* shift buffer: keep last six terms */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }

    return buf[5];
}
