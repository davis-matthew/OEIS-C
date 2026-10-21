/*
 * A075912  Fourth column of triangle A075500.
 * https://oeis.org/A075912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 50*a(n-1) - 875*a(n-2) + 6250*a(n-3) - 15000*a(n-4) with
 *            a(0) = 1, a(1) = 50, a(2) = 1625, a(3) = 43750 (Fourth column of
 *            triangle A075500)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)50;
    if (n == 2) return (seq_t)1625;
    if (n == 3) return (seq_t)43750;

    /* Use a sliding window of the last four computed terms. */
    seq_t a0 = (seq_t)1;          /* a(n-4) */
    seq_t a1 = (seq_t)50;         /* a(n-3) */
    seq_t a2 = (seq_t)1625;       /* a(n-2) */
    seq_t a3 = (seq_t)43750;      /* a(n-1) */
    seq_t ai;                    /* current a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)50 * a3
            - (seq_t)875 * a2
            + (seq_t)6250 * a1
            - (seq_t)15000 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
