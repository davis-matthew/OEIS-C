/*
 * A202825  Expansion of e.g.f.: exp( (1+x)^5 - 1 ).
 * https://oeis.org/A202825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence * a(0)=1, a(1)=5, a(2)=45, a(3)=485, a(4)=6145, *
 *            a(n+5) = 5*a(n+4) + 20*(n+4)*a(n+3) + 30*(n+3)*(n+4)*a(n+2) * +
 *            20*(n+2)*(n+3)*(n+4)*a(n+1) + 5*(n+1)*(n+2)*(n+3)*(n+4)*a(n) *
 *            for n ≥ 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial values of the sequence */
    static const seq_t init[5] = {
        (seq_t)1,
        (seq_t)5,
        (seq_t)45,
        (seq_t)485,
        (seq_t)6145
    };

    if (n < 5) {
        return init[n];
    }

    /* Sliding window: w0 = a(k), w1 = a(k+1), …, w4 = a(k+4) */
    seq_t w0 = init[0];
    seq_t w1 = init[1];
    seq_t w2 = init[2];
    seq_t w3 = init[3];
    seq_t w4 = init[4];

    for (idx_t k = 0; k + 5 <= n; ++k) {
        seq_t k_ts = (seq_t)k;

        /* Compute a(k+5) using the recurrence */
        seq_t t1 = (seq_t)5 * w4;
        seq_t t2 = (seq_t)20 * (k_ts + (seq_t)4) * w3;
        seq_t t3 = (seq_t)30 * (k_ts + (seq_t)3) * (k_ts + (seq_t)4) * w2;
        seq_t t4 = (seq_t)20 * (k_ts + (seq_t)2) * (k_ts + (seq_t)3) *
                   (k_ts + (seq_t)4) * w1;
        seq_t t5 = (seq_t)5 * (k_ts + (seq_t)1) * (k_ts + (seq_t)2) *
                   (k_ts + (seq_t)3) * (k_ts + (seq_t)4) * w0;

        seq_t new_val = t1 + t2 + t3 + t4 + t5;

        /* Shift the window for the next iteration */
        w0 = w1;
        w1 = w2;
        w2 = w3;
        w3 = w4;
        w4 = new_val;
    }

    return w4; /* w4 now holds a(n) */
}
