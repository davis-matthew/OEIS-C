/*
 * A080422  a(n) = (n+1)*(n+2)*(n+3)*(n+12)*3^n/72.
 * https://oeis.org/A080422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(n+12)*3^n/72
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 13, u64 n <= 31, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for n≥5: a(n)=15*a(n‑1)-90*a(n‑2)+270*a(n‑3)-405*a(n‑4)+243*a(n‑5) */

seq_t a(idx_t n)
{
    /* For n < 5 use the explicit formula */
    if (n < 5) {
        seq_t pow3 = 1;
        for (idx_t i = 0; i < n; ++i)
            pow3 *= (seq_t)3;
        seq_t term = ( (seq_t)(n + 1) *
                      (seq_t)(n + 2) *
                      (seq_t)(n + 3) *
                      (seq_t)(n + 12) );
        return term * pow3 / (seq_t)72;
    }

    /* Compute the first five terms (0..4) with the formula */
    seq_t init[5];
    for (idx_t i = 0; i < 5; ++i) {
        seq_t pow3 = 1;
        for (idx_t j = 0; j < i; ++j)
            pow3 *= (seq_t)3;
        init[i] = ( (seq_t)(i + 1) *
                    (seq_t)(i + 2) *
                    (seq_t)(i + 3) *
                    (seq_t)(i + 12) );
        init[i] = init[i] * pow3 / (seq_t)72;
    }

    /* Sliding window for the recurrence:
       last5[0] = a(k-1), last5[1] = a(k-2), …, last5[4] = a(k-5)   */
    seq_t last5[5];
    last5[0] = init[4];
    last5[1] = init[3];
    last5[2] = init[2];
    last5[3] = init[1];
    last5[4] = init[0];

    for (idx_t k = 5; k <= n; ++k) {
        seq_t t1 = (seq_t)15  * last5[0];
        seq_t t2 = (seq_t)270 * last5[2];
        seq_t t3 = (seq_t)243 * last5[4];
        seq_t sum = t1 + t2 + t3;

        seq_t t4 = (seq_t)90  * last5[1];
        seq_t t5 = (seq_t)405 * last5[3];
        sum = sum - t4;
        sum = sum - t5;

        /* shift window */
        last5[4] = last5[3];
        last5[3] = last5[2];
        last5[2] = last5[1];
        last5[1] = last5[0];
        last5[0] = sum;
    }

    return last5[0];
}
