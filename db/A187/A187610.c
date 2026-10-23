/*
 * A187610  Number of 6-step one space for components leftwards or up, two
 *          space for components rightwards or down asymmetric quasi-bishop's
 *          tours (antidiagonal moves become knight moves) on an n X n board
 *          summed over all starting positions.
 * https://oeis.org/A187610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 680*n^2 - 7188*n + 18314 for n >= 10 For n <= 12, use the
 *            exact initial values listed in the OEIS entry
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Initial values for n = 1 … 12 (a(1) = 0, a(2) = 0, ... ) */
    static const seq_t init[13] = {
        0, /* dummy for index 0 */
        0, /* a(1) */
        0, /* a(2) */
        0, /* a(3) */
        0, /* a(4) */
        96,   /* a(5) */
        711,  /* a(6) */
        2083, /* a(7) */
        4758, /* a(8) */
        8979, /* a(9) */
        14434,/* a(10) */
        21526,/* a(11) */
        29978 /* a(12) */
    };

    if (n == 0) {
        return 0;          /* Undefined for n = 0; return 0 for safety */
    }
    if (n <= 12) {
        return init[n];
    }

    /* Use the quadratic formula: a(n) = 680*n^2 - 7188*n + 18314
       Compute as (680*n^2 + 18314) - 7188*n to avoid unsigned underflow */
    const seq_t nn = (seq_t)n;
    const seq_t term = 680 * nn * nn;         /* 680*n^2  */
    return term + 18314 - 7188 * nn;
}
