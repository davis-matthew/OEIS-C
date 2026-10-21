/*
 * A075923  Fourth column of triangle A075502.
 * https://oeis.org/A075923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A075923 (OEIS) – Fourth column of triangle A075502.
 * Formula implemented via its 4-term linear recurrence:
 * a(n) = 70*a(n-1) - 1715*a(n-2) + 17150*a(n-3) - 57624*a(n-4)
 * with a(0)=1, a(1)=70, a(2)=3185, a(3)=120050.
 * This recurrence follows directly from the generating function
 * 1/[(1‑7x)(1‑14x)(1‑21x)(1‑28x)].
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial values for n = 0 .. 3 */
    static const seq_t init[4] = { (seq_t)1, (seq_t)70, (seq_t)3185, (seq_t)120050 };

    if (n < 4)
        return init[n];

    /* rolling buffer for the last four terms */
    seq_t a0 = init[0];   /* a(n‑4) */
    seq_t a1 = init[1];   /* a(n‑3) */
    seq_t a2 = init[2];   /* a(n‑2) */
    seq_t a3 = init[3];   /* a(n‑1) */
    seq_t an;             /* a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        /* compute 70*a(n‑1) + 17150*a(n‑3) */
        seq_t term1 = ((seq_t)70) * a3;
        seq_t term2 = ((seq_t)17150) * a1;
        seq_t sum1  = term1 + term2;

        /* compute 1715*a(n‑2) + 57624*a(n‑4) */
        seq_t term3 = ((seq_t)1715) * a2;
        seq_t term4 = ((seq_t)57624) * a0;
        seq_t sum2  = term3 + term4;

        /* a(n) = sum1 - sum2  (non‑negative by the recurrence) */
        an = sum1 - sum2;

        /* shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
