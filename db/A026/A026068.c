/*
 * A026068  (d(n)-r(n))/5, where d = A026066 and r is the periodic sequence
 *          with fundamental period (0,3,1,0,1).
 * https://oeis.org/A026068
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) -3*a(n-2) +a(n-3) +a(n-5) -3*a(n-6) +3*a(n-7)
 *            -a(n-8); Initial values (n = 7…14): a(7)=21, a(8)=33, a(9)=49,
 *            a(10)=68, a(11)=90, a(12)=116, a(13)=145, a(14)=179
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 7 the value is undefined. Return 0 for convenience. */
    if (n < 7) return 0;

    /* Hard‑coded initial terms */
    static const seq_t init[8] = {
        (seq_t)21,  (seq_t)33,  (seq_t)49,  (seq_t)68,
        (seq_t)90,  (seq_t)116, (seq_t)145, (seq_t)179
    };

    /* If the requested index is one of the initial terms, return it directly. */
    if (n <= 14) return init[n - 7];

    /* Compute a(n) using the 8‑term recurrence. */
    seq_t vals[8];
    for (int i = 0; i < 8; ++i) vals[i] = init[i];  /* vals[0] = a(7), ..., vals[7] = a(14) */

    for (idx_t k = 15; k <= n; ++k) {
        /* Compute a(k) from the previous eight values. */
        seq_t new_val = vals[7] * (seq_t)3   /* a(k-1) */
                     - vals[6] * (seq_t)3   /* a(k-2) */
                     + vals[5]              /* a(k-3) */
                     + vals[3]              /* a(k-5) */
                     - vals[2] * (seq_t)3   /* a(k-6) */
                     + vals[1] * (seq_t)3   /* a(k-7) */
                     - vals[0];             /* a(k-8) */

        /* Shift the array left, discarding a(k-8) and making room for the new value. */
        for (int i = 0; i < 7; ++i) vals[i] = vals[i + 1];
        vals[7] = new_val;
    }

    return vals[7];   /* a(n) after the loop ends */
}
