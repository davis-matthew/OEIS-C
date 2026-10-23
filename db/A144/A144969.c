/*
 * A144969  Stirling numbers of second kind S(n,n-6).
 * https://oeis.org/A144969
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Stirling number of the second kind S(n, n-6) (offset 6,
 *            so a(6) is first term). The recurrence in the difference
 *            variable D = n - k is: a_D(n) = a_D(n-1) + (n-D) * a_{D-1}(n-1),
 *            with a_0(n)=1
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 19, u64 n <= 102,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n)=0 for n<6, as the sequence is defined from offset 6. */
    if (n < 6) {
        return (seq_t)0;
    }

    /* f[D] will hold a_D(current n). We only need D = 0..6. */
    seq_t f[7] = {0};
    f[0] = 1;                     /* S(0,0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        /* Update from largest D to avoid overwriting needed values. */
        for (int D = 6; D >= 1; --D) {
            seq_t prev = f[D];                         /* a_D(n-1) */
            seq_t mult = (seq_t)(i - (idx_t)D) * f[D - 1]; /* (n-D) * a_{D-1}(n-1) */
            f[D] = prev + mult;
        }
        f[0] = 1; /* S(i,i) = 1 for all i ≥ 0 */
    }

    return f[6];   /* This is S(n, n-6). */
}
