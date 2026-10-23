/*
 * A134815  Expansion of g.f.
 *          -x*(3*x^7+2*x^6+x^5+15*x^4+3*x^3-x^2+15*x-4)/((x^4+x-1)*(x^4+5*x-1)).
 * https://oeis.org/A134815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 6*a(n-1) -
 *            5*a(n-2) + 2*a(n-4) - 6*a(n-5) - a(n-8) * for n ≥ 9, with
 *            initial values * a(1)=4, a(2)=9, a(3)=35, a(4)=162, * a(5)=790,
 *            a(6)=3923, a(7)=19602, a(8)=98108
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial eight terms (offset 1). */
    static const seq_t init[8] = {4, 9, 35, 162, 790, 3923, 19602, 98108};

    if (n <= 8) return init[n - 1];

    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i) buf[i] = init[i];

    for (idx_t m = 9; m <= n; ++m)
    {
        /* Buffer layout: buf[0] = a(n-8), buf[1] = a(n-7), ..., buf[7] = a(n-1). */
        seq_t a1 = buf[7];   /* a(n‑1)   */
        seq_t a2 = buf[6];   /* a(n‑2)   */
        seq_t a4 = buf[4];   /* a(n‑4)   */
        seq_t a5 = buf[3];   /* a(n‑5)   */
        seq_t a8 = buf[0];   /* a(n‑8)   */

        /* Compute a(n) = 6*a1 - 5*a2 + 2*a4 - 6*a5 - a8 in a safe way. */
        seq_t sum_pos = 6 * a1 + 2 * a4;
        seq_t sum_neg = 5 * a2 + 6 * a5 + a8;
        seq_t val = sum_pos - sum_neg;  /* safe: sum_pos ≥ sum_neg */

        /* Shift the window: drop the oldest term and add the new term. */
        for (size_t i = 0; i < 7; ++i) buf[i] = buf[i + 1];
        buf[7] = val;
    }

    return buf[7];
}
