/*
 * A236349  Number of (n+1)X(1+1) 0..2 arrays colored with the sum of the
 *          upper and lower median values of each 2X2 subblock.
 * https://oeis.org/A236349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 9*a(n‑2) + 64*a(n‑3)
 *            + 304*a(n‑4) + 596*a(n‑5) + 912*a(n‑6) + 176*a(n‑7) –
 *            2528*a(n‑8) – 3648*a(n‑9) – 2048*a(n‑10) with the initial values
 *            a(1)=81, a(2)=462, a(3)=2588, a(4)=14952, a(5)=86800,
 *            a(6)=498976, a(7)=2868608, a(8)=16528896, a(9)=95237312, a(...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values (index 0 unused) */
    static const seq_t init[11] = {
        (seq_t)0ULL,      /* placeholder for a(0) */
        (seq_t)81ULL,
        (seq_t)462ULL,
        (seq_t)2588ULL,
        (seq_t)14952ULL,
        (seq_t)86800ULL,
        (seq_t)498976ULL,
        (seq_t)2868608ULL,
        (seq_t)16528896ULL,
        (seq_t)95237312ULL,
        (seq_t)548375168ULL
    };

    /* Return known values directly for n ≤ 10. */
    if (n <= 10) {
        return init[n];
    }

    /* last[0] … last[9] hold a(n‑10) … a(n‑1) for the current step. */
    seq_t last[10];
    last[0] = init[1];
    last[1] = init[2];
    last[2] = init[3];
    last[3] = init[4];
    last[4] = init[5];
    last[5] = init[6];
    last[6] = init[7];
    last[7] = init[8];
    last[8] = init[9];
    last[9] = init[10];

    for (idx_t i = 11; i <= n; ++i) {
        /* Compute the positive part of the recurrence. */
        seq_t pos = 0;
        pos += (seq_t)9   * last[8];
        pos += (seq_t)64  * last[7];
        pos += (seq_t)304 * last[6];
        pos += (seq_t)596 * last[5];
        pos += (seq_t)912 * last[4];
        pos += (seq_t)176 * last[3];

        /* Compute the negative part of the recurrence. */
        seq_t neg = 0;
        neg += (seq_t)2528 * last[2];
        neg += (seq_t)3648 * last[1];
        neg += (seq_t)2048 * last[0];

        seq_t new_val = pos;
        new_val -= neg;   /* final coefficient is positive */

        /* Shift array left and store the newly computed value. */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = last[6];
        last[6] = last[7];
        last[7] = last[8];
        last[8] = last[9];
        last[9] = new_val;
    }

    return last[9];
}
