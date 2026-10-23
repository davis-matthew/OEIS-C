/*
 * A185515  Number of (n+1) X 4 0..2 arrays with every 2 X 2 subblock
 *          singular.
 * https://oeis.org/A185515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) - 47*a(n-2) - 1009*a(n-3) + 5624*a(n-4) -
 *            3060*a(n-5) - 27360*a(n-6) + 36864*a(n-7) Initial values:
 *            a(1)=593, a(2)=12361, a(3)=154561, a(4)=2490449, a(5)=35659489,
 *            a(6)=541370505, a(7)=7998109489
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /*  Offset starts at 1; undefined for n<1. */
    if (n < 1u) return (seq_t)0;

    static const seq_t init[7] = {
        593ULL,        /* a(1) */
        12361ULL,      /* a(2) */
        154561ULL,     /* a(3) */
        2490449ULL,    /* a(4) */
        35659489ULL,   /* a(5) */
        541370505ULL,  /* a(6) */
        7998109489ULL  /* a(7) */
    };

    if (n <= 7u) return init[n - 1u];

    seq_t buf[7];
    for (int i = 0; i < 7; ++i) buf[i] = init[i];   /* buf[0]=a(1), ..., buf[6]=a(7) */

    for (idx_t k = 8u; k <= n; ++k) {
        seq_t sum_pos = (seq_t)21 * buf[6];
        sum_pos += (seq_t)5624 * buf[3];
        sum_pos += (seq_t)36864 * buf[0];

        seq_t sum_neg = (seq_t)47 * buf[5];
        sum_neg += (seq_t)1009 * buf[4];
        sum_neg += (seq_t)3060 * buf[2];
        sum_neg += (seq_t)27360 * buf[1];

        seq_t nxt = sum_pos - sum_neg;   /* safe: sum_pos > sum_neg */

        /* Shift buffer left: buf[0] becomes buf[1], ..., buf[5] becomes buf[6] */
        for (int i = 0; i < 6; ++i) buf[i] = buf[i + 1];
        buf[6] = nxt;
    }

    return buf[6];
}
