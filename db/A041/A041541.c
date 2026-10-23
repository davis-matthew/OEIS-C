/*
 * A041541  Denominators of continued fraction convergents to sqrt(287).
 * https://oeis.org/A041541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 576*a(n-4) - a(n-8) for n>7, a(0)=1, a(1)=1, a(2)=16,
 *            a(3)=17, a(4)=560, a(5)=577, a(6)=9215, a(7)=9792, a(8)=322559
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Initial terms */
    static const seq_t init[9] = {
        (seq_t)1,
        (seq_t)1,
        (seq_t)16,
        (seq_t)17,
        (seq_t)560,
        (seq_t)577,
        (seq_t)9215,
        (seq_t)9792,
        (seq_t)322559
    };

    if (n <= 8) {
        return init[n];
    }

    /* Window of the last 8 values needed for the recurrence.
       window[0] = a[i-8], window[1] = a[i-7], ..., window[7] = a[i-1] */
    seq_t window[8];
    window[0] = init[1];
    window[1] = init[2];
    window[2] = init[3];
    window[3] = init[4];
    window[4] = init[5];
    window[5] = init[6];
    window[6] = init[7];
    window[7] = init[8];

    seq_t result = init[8];  /* placeholder for a(n) */
    for (idx_t i = 9; i <= n; ++i) {
        seq_t nxt = ((seq_t)576) * window[4] - window[0];

        /* Shift window left by one position */
        for (int k = 0; k < 7; ++k) {
            window[k] = window[k + 1];
        }
        window[7] = nxt;   /* window now holds a[i-7]..a[i] */

        result = nxt;
    }
    return result;
}
