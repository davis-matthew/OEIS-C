/*
 * A188103  Number of (n+3)X10 binary arrays with every 4X4 subblock commuting
 *          with each horizontal and vertical neighbor 4X4 subblock.
 * https://oeis.org/A188103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 15, u32 n <= 60, u64 n <= 149, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-4) - 2*a(n-5) + a(n-6) + a(n-8) - a(n-9)
   for n > 17.
   Initial values for 1 ≤ n ≤ 18:

   a(1) = 17850
   a(2) = 2866
   a(3) = 4041
   a(4) = 5953
   a(5) = 8873
   a(6) = 10653
   a(7) = 11457
   a(8) = 12477
   a(9) = 14633
   a(10)= 17361
   a(11)= 20685
   a(12)= 25233
   a(13)= 34381
   a(14)= 45145
   a(15)= 57105
   a(16)= 71309
   a(17)= 94870
   a(18)=122775
*/

seq_t a(idx_t n)
{
    /* The first 18 terms are hard‑coded. */
    static const seq_t init[18] = {
        (seq_t)17850, (seq_t)2866, (seq_t)4041, (seq_t)5953,
        (seq_t)8873,  (seq_t)10653, (seq_t)11457, (seq_t)12477,
        (seq_t)14633, (seq_t)17361, (seq_t)20685, (seq_t)25233,
        (seq_t)34381, (seq_t)45145, (seq_t)57105, (seq_t)71309,
        (seq_t)94870, (seq_t)122775
    };

    if (n <= 18) {
        return init[n - 1];
    }

    /* Work with a sliding window of the last 9 terms:
       vals[0] = a(n-1), vals[1] = a(n-2), ..., vals[8] = a(n-9). */
    seq_t vals[9];
    /* Initialise for n = 19: a10 … a18 in reverse order. */
    for (idx_t i = 0; i < 9; ++i) {
        vals[i] = init[17 - i];  /* init[17] = a18, init[16] = a17, … */
    }

    /* Compute a(n) for all n >= 19 */
    for (idx_t cur = 19; cur <= n; ++cur) {
        seq_t next = vals[0] + vals[0];          /* 2*a(n-1) */
        next -= vals[1];                        /* - a(n-2) */
        next += vals[3];                        /* + a(n-4) */
        next -= vals[4] + vals[4];              /* - 2*a(n-5) */
        next += vals[5];                        /* + a(n-6) */
        next += vals[7];                        /* + a(n-8) */
        next -= vals[8];                        /* - a(n-9) */

        /* Shift the window for the next iteration. */
        for (idx_t i = 8; i > 0; --i) {
            vals[i] = vals[i - 1];
        }
        vals[0] = next;
    }

    return vals[0]; /* a(n) after the loop ends. */
}
