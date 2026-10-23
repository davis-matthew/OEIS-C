/*
 * A189275  Number of nX4 array permutations with each element not moved or
 *          moved diagonally or antidiagonally by one.
 * https://oeis.org/A189275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 12*a(n-1) - 7*a(n-2)
 *            - 78*a(n-3) + 365*a(n-4) + 67*a(n-5) + 269*a(n-6) - 269*a(n-7) -
 *            67*a(n-8) - 365*a(n-9) + 78*a(n-10) + 7*a(n-11) - 12*a(n-12) +
 *            a(n-13) with initial values a(1)=1, a(2)=25, …,
 *            a(13)=6293931930441
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 13 terms (n=1..13) */
    static const seq_t init[13] = {
        1ULL,          /* a(1)  */
        25ULL,         /* a(2)  */
        225ULL,        /* a(3)  */
        2704ULL,       /* a(4)  */
        29241ULL,      /* a(5)  */
        323761ULL,     /* a(6)  */
        3553225ULL,    /* a(7)  */
        39100009ULL,   /* a(8)  */
        429981696ULL,  /* a(9)  */
        4729725529ULL, /* a(10) */
        52022767225ULL,/* a(11) */
        572215089601ULL,/* a(12) */
        6293931930441ULL/* a(13) */
    };

    if (n <= 13)
        return init[n - 1];

    /* Working array of the last 13 values
       prev[0] = a(k-13), prev[12] = a(k-1) for current k */
    seq_t prev[13];
    for (int i = 0; i < 13; ++i)
        prev[i] = init[i];

    /* iterate from a(14) up to a(n) */
    for (idx_t k = 14; k <= n; ++k) {
        seq_t next = 12 * prev[12]
                   - 7  * prev[11]
                   - 78 * prev[10]
                   + 365 * prev[9]
                   + 67  * prev[8]
                   + 269 * prev[7]
                   - 269 * prev[6]
                   - 67  * prev[5]
                   - 365 * prev[4]
                   + 78  * prev[3]
                   + 7   * prev[2]
                   - 12  * prev[1]
                   + 1   * prev[0];

        /* shift: drop prev[0], shift left, append next */
        for (int i = 0; i < 12; ++i)
            prev[i] = prev[i + 1];
        prev[12] = next;
    }

    return prev[12];
}
