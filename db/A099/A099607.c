/*
 * A099607  Antidiagonal sums of triangle A099605, in which row n equals the
 *          inverse binomial transform of column n of the triangle A034870 of
 *          even-indexed rows of Pascal's triangle.
 * https://oeis.org/A099607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) satisfies the linear recurrence
 *     a(n) = 2*a(n-2) + 6*a(n-3) + 3*a(n-4) – 2*a(n-5) – a(n-6)
 * for n ≥ 6, with initial values
 *     a(0)=1, a(1)=2, a(2)=3, a(3)=9, a(4)=21, a(5)=40
 */

seq_t a(idx_t n)
{
    /* Return the initial five terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)21;
    if (n == 5) return (seq_t)40;

    /* a[0] … a[5] will hold the last six computed terms:
     * a[0] = a(n-6), a[1] = a(n-5), a[2] = a(n-4),
     * a[3] = a(n-3), a[4] = a(n-2), a[5] = a(n-1). */
    seq_t a[6] = {
        (seq_t)1, /* a(0) */
        (seq_t)2, /* a(1) */
        (seq_t)3, /* a(2) */
        (seq_t)9, /* a(3) */
        (seq_t)21,/* a(4) */
        (seq_t)40 /* a(5) */
    };

    seq_t an;

    for (idx_t i = 6; i <= n; ++i) {
        /* Compute a(i) using the recurrence. */
        an = (seq_t)2 * a[4]          /* a(i‑2) */
            + (seq_t)6 * a[3]          /* a(i‑3) */
            + (seq_t)3 * a[2]          /* a(i‑4) */
            - (seq_t)2 * a[1]          /* a(i‑5) */
            - a[0];                    /* a(i‑6) */

        /* Shift the array so that a[0]..a[5] become
         * a(n‑5)…a(n‑1) for the next iteration. */
        a[0] = a[1];
        a[1] = a[2];
        a[2] = a[3];
        a[3] = a[4];
        a[4] = a[5];
        a[5] = an;
    }

    return an;
}
