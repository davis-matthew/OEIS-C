/*
 * A220647  Number of ways to reciprocally link elements of an n X 2 array
 *          either to themselves or to exactly two king-move neighbors,
 *          without 3-loops.
 * https://oeis.org/A220647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) + 8*a(n-3) - 6*a(n-4) (n >= 5)
 *            Initial values: a(1) = 1, a(2) = 4, a(3) = 23, a(4) = 83
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset n = 1..4 directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)23;
    if (n == 4) return (seq_t)83;

    /* Sliding‑window variables for the last four terms */
    seq_t a1 = (seq_t)1;   /* a(n-4) */
    seq_t a2 = (seq_t)4;   /* a(n-3) */
    seq_t a3 = (seq_t)23;  /* a(n-2) */
    seq_t a4 = (seq_t)83;  /* a(n-1) */
    seq_t term;

    for (idx_t i = 5; i <= n; ++i) {
        term = a4 * (seq_t)3        /* 3*a(n-1) */
             + a3 * (seq_t)3        /* 3*a(n-2) */
             + a2 * (seq_t)8        /* 8*a(n-3) */
             - a1 * (seq_t)6;       /* -6*a(n-4) */

        /* Shift the window forward for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = term;
    }

    return a4;
}
