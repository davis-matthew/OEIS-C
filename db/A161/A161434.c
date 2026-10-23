/*
 * A161434  Number of 6-compositions.
 * https://oeis.org/A161434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+6)=12*a(n+5)-30*a(n+4)+40*a(n+3)-30*a(n+2)+12*a(n+1)-2*a(n).
 *            Initial values: a(0)=1, a(1)=6, a(2)=57, a(3)=524, a(4)=4803,
 *            a(5)=44022, a(6)=403495
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"          /* mul_small(seq_t, unsigned) */

seq_t a(idx_t n)
{
    static const seq_t init[7] = {
        (seq_t)1,      /* a(0) */
        (seq_t)6,      /* a(1) */
        (seq_t)57,     /* a(2) */
        (seq_t)524,    /* a(3) */
        (seq_t)4803,   /* a(4) */
        (seq_t)44022,  /* a(5) */
        (seq_t)403495  /* a(6) */
    };

    if (n < 7) return init[n];

    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i) buf[i] = init[i];

    for (idx_t i = 7; i <= n; ++i) {
        seq_t v1 = mul_small(buf[(i-1)%7], 12);
        seq_t v2 = mul_small(buf[(i-2)%7], 30);
        seq_t v3 = mul_small(buf[(i-3)%7], 40);
        seq_t v4 = mul_small(buf[(i-4)%7], 30);
        seq_t v5 = mul_small(buf[(i-5)%7], 12);
        seq_t v6 = mul_small(buf[(i-6)%7], 2);

        seq_t val = v1 - v2 + v3 - v4 + v5 - v6;
        buf[i%7] = val;
    }

    return buf[n%7];
}
