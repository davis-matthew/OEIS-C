/*
 * A233081  Number of nX7 0..3 arrays with no element x(i,j) adjacent to value
 *          3-x(i,j) horizontally or antidiagonally, top left element zero,
 *          and 1 appearing before 2 in row major order.
 * https://oeis.org/A233081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 693*a(n-1) - 44261*a(n-2) + 890611*a(n-3)
 *            - 7150206*a(n-4) + 22034516*a(n-5) - 15731352*a(n-6)
 * with initial values:
 * a(1) = 365, a(2) = 226895, a(3) = 141615905,
 * a(4) = 88417171553, a(5) = 55204524946337,
 * a(6) = 34467813715307081
 */
seq_t a(idx_t n)
{
    /* Initial 6 terms */
    static const seq_t init[6] = {
        (seq_t)365ULL,
        (seq_t)226895ULL,
        (seq_t)141615905ULL,
        (seq_t)88417171553ULL,
        (seq_t)55204524946337ULL,
        (seq_t)34467813715307081ULL
    };
    if (n <= 6)
        return init[n - 1];

    /* rolling buffer for the last 6 terms */
    seq_t last[6];
    for (idx_t i = 0; i < 6; ++i)
        last[i] = init[i];

    for (idx_t i = 7; i <= n; ++i) {
        /* compute a(i) from the recurrence   */
        seq_t pos = 0; /* sum of positive coeff contributions */
        seq_t neg = 0; /* sum of negative coeff contributions */

        pos += (seq_t)693ULL * last[5];        /* 693*a(i-1)   */
        neg += (seq_t)44261ULL * last[4];      /* 44261*a(i-2) */
        pos += (seq_t)890611ULL * last[3];      /* 890611*a(i-3) */
        neg += (seq_t)7150206ULL * last[2];     /* 7150206*a(i-4) */
        pos += (seq_t)22034516ULL * last[1];    /* 22034516*a(i-5) */
        neg += (seq_t)15731352ULL * last[0];    /* 15731352*a(i-6) */

        seq_t cur = pos - neg; /* final value is guaranteed non‑negative */

        /* rotate the buffer */
        for (idx_t k = 0; k < 5; ++k)
            last[k] = last[k + 1];
        last[5] = cur;
    }

    return last[5]; /* a(n) */
}
