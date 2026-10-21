/*
 * A204639  Number of (n+2) X 7 0..3 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically, diagonally
 *          or antidiagonally exactly three ways, and new values 0..3
 *          introduced in row major order.
 * https://oeis.org/A204639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -3*a(n-2) -4*a(n-3) -12*a(n-4) +12*a(n-5)
 *            +21*a(n-6) -9*a(n-7) -9*a(n-8) Initial values: a(1)=16855,
 *            a(2)=1860, a(3)=760, a(4)=1169, a(5)=3300, a(6)=3077, a(7)=9482,
 *            a(8)=35500, a(9)=109224, a(10)=410467
 * Range:     u8 none, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[10] = {
        16855ULL, 1860ULL, 760ULL, 1169ULL, 3300ULL,
        3077ULL, 9482ULL, 35500ULL, 109224ULL, 410467ULL
    };
    if (n <= 10) {
        return init[n-1];
    }

    seq_t prev[8];
    /* last 8 terms for n = 11: a10, a9, a8, a7, a6, a5, a4, a3 */
    prev[0] = init[9];
    prev[1] = init[8];
    prev[2] = init[7];
    prev[3] = init[6];
    prev[4] = init[5];
    prev[5] = init[4];
    prev[6] = init[3];
    prev[7] = init[2];

    for (idx_t i = 11; i <= n; ++i) {
        seq_t pos = 5 * prev[0];
        pos += 12 * prev[4];
        pos += 21 * prev[5];
        seq_t neg = 3 * prev[1];
        neg += 4 * prev[2];
        neg += 12 * prev[3];
        neg += 9 * prev[6];
        neg += 9 * prev[7];
        seq_t curr = pos - neg;

        /* shift the window */
        for (int j = 7; j > 0; --j) {
            prev[j] = prev[j-1];
        }
        prev[0] = curr;
    }
    return prev[0];
}
