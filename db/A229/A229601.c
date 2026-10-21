/*
 * A229601  Number of defective 3-colorings of an n X 3 0..2 array connected
 *          horizontally and vertically with exactly two mistakes, and colors
 *          introduced in row-major 0..2 order.
 * https://oeis.org/A229601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 81*a(n-2) + 185*a(n-3) - 162*a(n-4) +
 *            60*a(n-5) - 8*a(n-6) a(1) = 1, a(2) = 39, a(3) = 396, a(4) =
 *            3040, a(5) = 20714, a(6) = 131345, a(7) = 792929
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t init_vals[7] = {
    (seq_t)1, (seq_t)39, (seq_t)396, (seq_t)3040,
    (seq_t)20714, (seq_t)131345, (seq_t)792929
};

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0; safe fallback */

    if (n <= 7) return init_vals[n - 1];

    /* Window of the last six terms: v[0]=a(n-1), v[1]=a(n-2), …, v[5]=a(n-6) */
    seq_t v[6] = {
        init_vals[6], /* a(7)  */
        init_vals[5], /* a(6)  */
        init_vals[4], /* a(5)  */
        init_vals[3], /* a(4)  */
        init_vals[2], /* a(3)  */
        init_vals[1]  /* a(2)  */
    };

    for (idx_t i = 8; i <= n; ++i) {
        seq_t new_val =
            (seq_t)15 * v[0]
          - (seq_t)81 * v[1]
          + (seq_t)185 * v[2]
          - (seq_t)162 * v[3]
          + (seq_t)60 * v[4]
          - (seq_t)8  * v[5];
        /* Shift the window */
        v[5] = v[4];
        v[4] = v[3];
        v[3] = v[2];
        v[2] = v[1];
        v[1] = v[0];
        v[0] = new_val;
    }

    return v[0]; /* This is a(n) */
}
