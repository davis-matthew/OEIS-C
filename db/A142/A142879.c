/*
 * A142879  a(n) = 5*a(n-3) - a(n-6) with terms 1..6 as 0, 1, 2, 5, 7, 9.
 * https://oeis.org/A142879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-3) - a(n-6) with a(1)=0, a(2)=1, a(3)=2, a(4)=5,
 *            a(5)=7, a(6)=9
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 43, u64 n <= 86, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A142879.
   The implementation uses the plain linear‑recurrence recurrence
   and iterates from the first non‑trivial term (n=7) up to n. */
seq_t a(idx_t n)
{
    /* Handle the first six terms directly. */
    if (n <= 6) {
        static const seq_t init[6] = {0, 1, 2, 5, 7, 9};
        return init[n - 1];
    }

    /* Window of the last six values:
       cyc[0] = a(i-6), cyc[1] = a(i-5), …, cyc[5] = a(i-1)   */
    seq_t cyc[6] = {0, 1, 2, 5, 7, 9};

    for (idx_t i = 7; i <= n; ++i) {
        /* a(i) = 5*a(i-3) - a(i-6)  →  5*cyc[3] - cyc[0] */
        seq_t an = 5 * cyc[3] - cyc[0];

        /* Shift the window left by one position. */
        for (int k = 0; k < 5; ++k)
            cyc[k] = cyc[k + 1];
        cyc[5] = an;
    }

    return cyc[5];
}
