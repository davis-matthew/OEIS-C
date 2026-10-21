/*
 * A224144  Number of n X 6 0..1 arrays with rows and antidiagonals unimodal
 *          and columns nondecreasing.
 * https://oeis.org/A224144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) for n > 10 Initial values: a(1)=22,
 *            a(2)=148, a(3)=554, a(4)=1573, a(5)=3827, a(6)=8375, a(7)=16885,
 *            a(8)=31841, a(9)=56783, a(10)=96579
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 66, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First ten terms */
    static const seq_t init_vals[11] = {
        0ULL,          /* unused, index 0 is undefined */
        22ULL,         /* a(1) */
        148ULL,        /* a(2) */
        554ULL,        /* a(3) */
        1573ULL,       /* a(4) */
        3827ULL,       /* a(5) */
        8375ULL,       /* a(6) */
        16885ULL,      /* a(7) */
        31841ULL,      /* a(8) */
        56783ULL,      /* a(9) */
        96579ULL       /* a(10) */
    };

    if (n <= 10)    /* direct lookup for the first ten terms */
        return init_vals[n];

    /* Storage for the last seven values needed by the recurrence.
       prev[0]  = a(n-7)
       prev[1]  = a(n-6)
       ...
       prev[6]  = a(n-1)   */
    seq_t prev[7] = {
        init_vals[4],   /* a(4)  */
        init_vals[5],   /* a(5)  */
        init_vals[6],   /* a(6)  */
        init_vals[7],   /* a(7)  */
        init_vals[8],   /* a(8)  */
        init_vals[9],   /* a(9)  */
        init_vals[10]   /* a(10) */
    };

    seq_t new_val = 0;

    for (idx_t i = 11; i <= n; ++i) {
        /* Compute a(i) from the seven previous values */
        new_val = 7 * prev[6]
                  - 21 * prev[5]
                  + 35 * prev[4]
                  - 35 * prev[3]
                  + 21 * prev[2]
                  - 7  * prev[1]
                  + prev[0];

        /* Shift the window so that prev[0] becomes a(i-6),
           prev[1] becomes a(i-5), ... prev[6] becomes a(i)  */
        for (int k = 0; k < 6; ++k)
            prev[k] = prev[k + 1];
        prev[6] = new_val;
    }

    return new_val;
}
