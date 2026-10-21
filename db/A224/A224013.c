/*
 * A224013  Number of 3 X n 0..2 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) * +
 *            21*a(n-5) - 7*a(n-6) + a(n-7) (for n>8) * Initial values:
 *            a(1)=27, a(2)=216, a(3)=868, a(4)=2661, * a(5)=6815, a(6)=15340,
 *            a(7)=31324, a(8)=59267
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 61, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Include the first 8 terms explicitly. */
    static const seq_t init[8] = {
        27ULL,            /* a(1) */
        216ULL,           /* a(2) */
        868ULL,           /* a(3) */
        2661ULL,          /* a(4) */
        6815ULL,          /* a(5) */
        15340ULL,         /* a(6) */
        31324ULL,         /* a(7) */
        59267ULL          /* a(8) */
    };

    if (n <= 8)
        return init[n - 1];

    /* Sliding window of the last 7 computed terms:
     *   a1 holds a(n-1), a2 holds a(n-2), ..., a7 holds a(n-7).
     */
    seq_t a1 = init[7]; /* a(8) */
    seq_t a2 = init[6]; /* a(7) */
    seq_t a3 = init[5]; /* a(6) */
    seq_t a4 = init[4]; /* a(5) */
    seq_t a5 = init[3]; /* a(4) */
    seq_t a6 = init[2]; /* a(3) */
    seq_t a7 = init[1]; /* a(2) */

    for (idx_t i = 9; i <= n; ++i) {
        seq_t next = 7 * a1 - 21 * a2 + 35 * a3 - 35 * a4
                     + 21 * a5 - 7 * a6 + a7;

        /* Shift the window for the next iteration. */
        a7 = a6;
        a6 = a5;
        a5 = a4;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = next;
    }

    return a1;
}
