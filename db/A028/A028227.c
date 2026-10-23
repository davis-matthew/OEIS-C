/*
 * A028227  Expansion of 1/((1-7x)(1-9x)(1-11x)(1-12x)).
 * https://oeis.org/A028227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8*12^(n+3) - 15*11^(n+3) + 10*9^(n+3) - 3*7^(n+3)) / 120
 *            Recurrence: a(n) = 39*a(n-1) – 563*a(n-2) + 3561*a(n-3) –
 *            8316*a(n-4) Initial terms: a(0)=1, a(1)=39, a(2)=958, a(3)=18966
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    static const seq_t init[4] = {(seq_t)1, (seq_t)39, (seq_t)958, (seq_t)18966};
    if (n < 4)
        return init[n];

    /* Use a sliding window of the last four terms */
    seq_t a0 = init[0];          /* a(n-4) */
    seq_t a1 = init[1];          /* a(n-3) */
    seq_t a2 = init[2];          /* a(n-2) */
    seq_t a3 = init[3];          /* a(n-1) */
    seq_t an = 0;                /* a(n)  will be computed */

    for (idx_t i = 4; i <= n; ++i) {
        /* Positives: 39*a3 + 3561*a1
           Negatives:  563*a2 + 8316*a0
           Compute separately to avoid underflow on unsigned types */
        seq_t pos = (seq_t)39 * a3 + (seq_t)3561 * a1;
        seq_t neg = (seq_t)563 * a2 + (seq_t)8316 * a0;
        an = pos - neg;

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
