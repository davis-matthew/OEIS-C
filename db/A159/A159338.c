/*
 * A159338  Transform of the finite sequence (1, 0, -1, 0, 1, 0, -1) by the
 *          T_{1,0} transformation (see link).
 * https://oeis.org/A159338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) (n ≥ 9) Initial values:
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=11, a(4)=27, a(5)=61, a(6)=140,
 *            a(7)=327, a(8)=761
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* first nine terms */
    static const seq_t init[9] = {
        (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)11,
        (seq_t)27, (seq_t)61, (seq_t)140, (seq_t)327,
        (seq_t)761
    };

    if (n <= 8) {
        return init[n];
    }

    /* sliding window: t0 = a(n-3), t1 = a(n-2), t2 = a(n-1) */
    seq_t t0 = init[6];      /* a(6) */
    seq_t t1 = init[7];      /* a(7) */
    seq_t t2 = init[8];      /* a(8) */
    seq_t next;

    for (idx_t i = 9; i <= n; ++i) {
        next = seq_mul(t2, (seq_t)3); /* 3*a(n-1) */
        next += t0;                  /* + a(n-3) */
        next -= seq_mul(t1, (seq_t)2);/* - 2*a(n-2) */

        /* advance window */
        t0 = t1;
        t1 = t2;
        t2 = next;
    }

    return t2;
}
