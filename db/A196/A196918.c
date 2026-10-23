/*
 * A196918  Number of n X 4 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 1,3,2,4,0
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + a(n-3) - a(n-4) - a(n-5) - 5*a(n-6)
 *            * - 8*a(n-7) - 3*a(n-8) for n > 9 * Initial values: * a(1) = 1 *
 *            a(2) = 4 * a(3) = 12 * a(4) = 37 * a(5) = 114 * a(6) = 335 *
 *            a(7) = 1003 * a(8) = 3051 * a(9) = 9290
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first nine terms */
    static const seq_t init[9] = {
        1, 4, 12, 37, 114, 335, 1003, 3051, 9290
    };
    if (n <= 9) {
        return init[n - 1];
    }

    seq_t last[9];
    for (idx_t i = 0; i < 9; ++i) {
        last[i] = init[i];
    }

    for (idx_t cur = 10; cur <= n; ++cur) {
        seq_t newv = 4 * last[8]
                   - 3 * last[7]
                   +   last[6]
                   -   last[5]
                   -   last[4]
                   - 5 * last[3]
                   - 8 * last[2]
                   - 3 * last[1];
        /* shift left: drop a(cur-8) */
        for (idx_t j = 0; j < 8; ++j) {
            last[j] = last[j + 1];
        }
        last[8] = newv;
    }

    return last[8];
}
