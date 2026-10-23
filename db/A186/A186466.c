/*
 * A186466  Number of (n+1) X 4 0..2 arrays with every 2 X 2 subblock
 *          commuting with each of its horizontal and vertical 2 X 2 subblock
 *          neighbors.
 * https://oeis.org/A186466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 4*a(n-3) + 4*a(n-4) - 2*a(n-5) for n
 *            > 12 Initial values: a(1) = 277, a(2) = 243, a(3) = 942, a(4) =
 *            1203, a(5) = 3190, a(6) = 4023, a(7) = 9982, a(8) = 13985, a(9)
 *            = 32390, a(10) = 48553, a(11) = 105436, a(12) = 165842
 * Range:     u8 none, u16 n <= 10, u32 n <= 28, u64 n <= 65, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive indices */
    if (n < 1)
        return 0;

    /* Constants for indices 1..12 */
    static const seq_t init[12] = {
        (seq_t)277,   /* a(1) */
        (seq_t)243,   /* a(2) */
        (seq_t)942,   /* a(3) */
        (seq_t)1203,  /* a(4) */
        (seq_t)3190,  /* a(5) */
        (seq_t)4023,  /* a(6) */
        (seq_t)9982,  /* a(7) */
        (seq_t)13985, /* a(8) */
        (seq_t)32390, /* a(9) */
        (seq_t)48553, /* a(10) */
        (seq_t)105436,/* a(11) */
        (seq_t)165842 /* a(12) */
    };

    if (n <= 12)
        return init[n - 1];

    /* Use the 5 most recent terms to apply the recurrence. */
    seq_t v1 = init[11]; /* a[n-1] = a[12] for n == 13 */
    seq_t v2 = init[10]; /* a[n-2] = a[11] */
    seq_t v3 = init[9];  /* a[n-3] = a[10] */
    seq_t v4 = init[8];  /* a[n-4] = a[9]  */
    seq_t v5 = init[7];  /* a[n-5] = a[8]  */

    idx_t i;
    for (i = 13; i <= n; ++i) {
        seq_t next = (2 * v1) + v2 - (4 * v3) + (4 * v4) - (2 * v5);
        v5 = v4;
        v4 = v3;
        v3 = v2;
        v2 = v1;
        v1 = next;
    }
    return v1;
}
