/*
 * A159347  Transform of the finite sequence (1, 0, -1) by the T_{0,0}
 *          transformation.
 * https://oeis.org/A159347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=1, a(3)=4, a(4)=10; a(n)=3*a(n‑1) –
 *            2*a(n‑2) + a(n‑3) for n≥5
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial five values */
    static const seq_t init[5] = { (seq_t)1, (seq_t)1, (seq_t)1,
                                  (seq_t)4, (seq_t)10 };

    if (n < 5) return init[n];

    /* start with a(2), a(3), a(4) */
    seq_t a_nm3 = init[2]; /* a(n‑3) */
    seq_t a_nm2 = init[3]; /* a(n‑2) */
    seq_t a_nm1 = init[4]; /* a(n‑1) */
    seq_t a_n;            /* a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        /* a(n) = 3*a(n‑1) - 2*a(n‑2) + a(n‑3) */
        seq_t three = a_nm1 + a_nm1 + a_nm1;          /* 3*a(n‑1) */
        seq_t two   = a_nm2 + a_nm2;                  /* 2*a(n‑2) */
        a_n = three + a_nm3 - two;                   /* 3*a(n‑1) + a(n‑3) - 2*a(n‑2) */

        /* shift for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
