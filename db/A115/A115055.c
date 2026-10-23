/*
 * A115055  Lower level digraph derived from a voltage graph.
 * https://oeis.org/A115055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 27, u32 n <= 51, u64 n <= 97, u128 n <= 189,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-3)+3*a(n-4)+3*a(n-5)+a(n-6)  (n ≥ 7)
   with initial values: a(1)=0, a(2)=1, a(3)=0, a(4)=0, a(5)=1, a(6)=3 */

seq_t a(idx_t n)
{
    /* constant initial values */
    static const seq_t init[6] = {0, 1, 0, 0, 1, 3};

    if (n <= 6)                    /* return the constant terms */
        return init[(int)(n - 1)];

    /* sliding window: w[0] = a(current‑6), w[1] = a(current‑5),
       w[2] = a(current‑4), w[3] = a(current‑3), w[4] = a(current‑2),
       w[5] = a(current‑1) */
    seq_t w[6];
    for (int i = 0; i < 6; ++i)
        w[i] = init[i];

    /* iterate from a(7) up to a(n) */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = w[3] + 3 * w[2] + 3 * w[1] + w[0];  /* a(i)   */

        /* shift window left by one position */
        w[0] = w[1];
        w[1] = w[2];
        w[2] = w[3];
        w[3] = w[4];
        w[4] = w[5];
        w[5] = next;                                 /* a(i)   */
    }

    return w[5];   /* w[5] holds a(n) after the loop */
}
