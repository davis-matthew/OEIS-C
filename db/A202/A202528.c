/*
 * A202528  Number of (n+2) X 6 binary arrays avoiding patterns 000 and 101 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-3) - 2*a(n-4) - 2*a(n-5) + a(n-7) for
 *            n>8. Initial values: a(1)=890, a(2)=2232, a(3)=4669, a(4)=9746,
 *            a(5)=21297, a(6)=45899, a(7)=97936, a(8)=210562
 * Range:     u8 none, u16 n <= 6, u32 n <= 20, u64 n <= 49, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[9] = {
        0ULL,          /* unused, index 0 */
        890ULL,        /* a(1) */
        2232ULL,       /* a(2) */
        4669ULL,       /* a(3) */
        9746ULL,       /* a(4) */
        21297ULL,      /* a(5) */
        45899ULL,      /* a(6) */
        97936ULL,      /* a(7) */
        210562ULL      /* a(8) */
    };

    if (n <= 8)
        return init[n];

    /* prev[0] .. prev[6] hold a(n-7) .. a(n-1) for the current step */
    seq_t prev[7];
    prev[0] = init[2];   /* a(2) = a(n-7) when n=9 */
    prev[1] = init[3];   /* a(3)         */
    prev[2] = init[4];   /* a(4)         */
    prev[3] = init[5];   /* a(5)         */
    prev[4] = init[6];   /* a(6)         */
    prev[5] = init[7];   /* a(7)         */
    prev[6] = init[8];   /* a(8)         */

    for (idx_t i = 9; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + 2*a(i-3) - 2*a(i-4) - 2*a(i-5) + a(i-7) */
        seq_t next = 2 * prev[6] +          /* a(i-1) */
                    2 * prev[4] +          /* a(i-3) */
                    prev[0];              /* a(i-7) */

        next -= 2 * prev[3];               /* a(i-4) */
        next -= 2 * prev[2];               /* a(i-5) */

        /* Shift window to prepare for next iteration */
        for (int j = 0; j < 6; ++j)
            prev[j] = prev[j + 1];
        prev[6] = next;
    }

    return prev[6];
}
