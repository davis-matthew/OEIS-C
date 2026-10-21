/*
 * A224739  Number of (n+1) X 3 0..1 matrices with each 2 X 2 permanent equal.
 * https://oeis.org/A224739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 7*a(n-2) - 34*a(n-3) + 59*a(n-4) + 27*a(n-5) -
 *            87*a(n-6) + 36*a(n-7) for n ≥ 8. Initial values (offset 1):
 *            a(1)=38, a(2)=152, a(3)=565, a(4)=2326, a(5)=9554, a(6)=40297,
 *            a(7)=170754
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Small table of the first seven terms. */
    static const seq_t init[8] = {
        (seq_t)0,      /* unused 0‑index */
        (seq_t)38,     /* a(1) */
        (seq_t)152,    /* a(2) */
        (seq_t)565,    /* a(3) */
        (seq_t)2326,   /* a(4) */
        (seq_t)9554,   /* a(5) */
        (seq_t)40297,  /* a(6) */
        (seq_t)170754  /* a(7) */
    };

    if (n <= 7)                 /* offset 1: return known values */
        return init[n];

    /* Keep the last seven values in a sliding window. */
    seq_t w[8];
    /* initialise with a(1) … a(7) */
    for (idx_t i = 1; i <= 7; ++i)
        w[i] = init[i];

    /* Compute terms from a(8) up to a(n). */
    for (idx_t i = 8; i <= n; ++i) {
        /* pos = 7*a(i‑1) + 59*a(i‑4) + 27*a(i‑3) + 36*a(i‑7) */
        seq_t pos = (seq_t)7 * w[7] + (seq_t)59 * w[4]
                  + (seq_t)27 * w[3] + (seq_t)36 * w[1];
        /* neg = 7*a(i‑2) + 34*a(i‑5) + 87*a(i‑6) */
        seq_t neg = (seq_t)7 * w[6] + (seq_t)34 * w[5]
                  + (seq_t)87 * w[2];
        /* a(i) = pos – neg (High always ≥ Low for this recurrence) */
        seq_t cur = pos - neg;

        /* slide the window: drop w[1], shift left, insert cur at w[7] */
        for (int k = 1; k < 7; ++k)
            w[k] = w[k + 1];
        w[7] = cur;
    }

    return w[7];  /* last element holds a(n) */
}
