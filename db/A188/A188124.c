/*
 * A188124  Number of strictly increasing arrangements of 5 nonzero numbers in
 *          -(n+3)..(n+3) with sum zero.
 * https://oeis.org/A188124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A188124
   Recurrence: a(n) = 2*a(n-1)-a(n-3)-2*a(n-5)+2*a(n-6)+a(n-8)-2*a(n-10)+a(n-11)
   Initial values (0 ≤ n ≤ 11):
   0,4,16,42,90,172,296,482,740,1092,1554,2154
   This implementation computes a(n) in O(n) time using only 12
   elements of workspace, avoiding any dynamic allocation so it
   compiles both as C11 and C++17. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* base values for n = 0 .. 11 */
    static const seq_t base[12] = {
        (seq_t)0,   /* a(0) */
        (seq_t)4,   /* a(1) */
        (seq_t)16,  /* a(2) */
        (seq_t)42,  /* a(3) */
        (seq_t)90,  /* a(4) */
        (seq_t)172, /* a(5) */
        (seq_t)296, /* a(6) */
        (seq_t)482, /* a(7) */
        (seq_t)740, /* a(8) */
        (seq_t)1092,/* a(9) */
        (seq_t)1554,/* a(10) */
        (seq_t)2154 /* a(11) */
    };

    if (n < 12U)
        return base[ n ];

    /* sliding window of the last 12 computed values:
       buf[0] = a(i-12), buf[11] = a(i-1)  (for current i)   */
    seq_t buf[12];
    for (idx_t k = 0; k < 12; ++k)
        buf[k] = base[k];

    for (idx_t i = 12; i <= n; ++i) {
        seq_t val = buf[11] + buf[11];           /* 2*a(i-1) */
        val = val - buf[ 9];                      /* -a(i-3)   */
        val = val - buf[ 7] - buf[ 7];            /* -2*a(i-5) */
        val = val + buf[ 6] + buf[ 6];            /* +2*a(i-6) */
        val = val + buf[ 4];                      /* +a(i-8)   */
        val = val - buf[ 2] - buf[ 2];            /* -2*a(i-10)*/
        val = val + buf[ 1];                      /* +a(i-11)  */

        /* shift left over the buffer */
        for (idx_t k = 0; k < 11; ++k)
            buf[k] = buf[k+1];
        buf[11] = val;
    }

    return buf[11];
}
