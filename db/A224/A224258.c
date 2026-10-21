/*
 * A224258  Number of n X 4 0..2 arrays with rows, diagonals and antidiagonals
 *          unimodal and columns nondecreasing.
 * https://oeis.org/A224258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) for n >
 *            11, with initial values a(1) … a(11) below
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 27, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms */
    static const seq_t init[11] = {
        (seq_t)46,  (seq_t)548,  (seq_t)3096,  (seq_t)12467,
        (seq_t)41012, (seq_t)116692, (seq_t)296646,
        (seq_t)688533, (seq_t)1482310, (seq_t)2995516,
        (seq_t)5735542
    };

    if (n <= 11) {
        return init[n - 1];
    }

    /* last[0] = a(n-9), last[8] = a(n-1) */
    seq_t last[9];
    for (idx_t i = 0; i < 9; ++i) {
        last[i] = init[2 + i];       /* init[2] = a(3) ... init[10] = a(11) */
    }

    seq_t new_val = 0;
    for (idx_t cur = 12; cur <= n; ++cur) {
        new_val = (seq_t)9  * last[8];
        new_val -= (seq_t)36 * last[7];
        new_val += (seq_t)84 * last[6];
        new_val -= (seq_t)126 * last[5];
        new_val += (seq_t)126 * last[4];
        new_val -= (seq_t)84  * last[3];
        new_val += (seq_t)36  * last[2];
        new_val -= (seq_t)9   * last[1];
        new_val += last[0];

        /* shift last[0..7] left, insert new_val at last[8] */
        for (idx_t i = 0; i < 8; ++i) {
            last[i] = last[i + 1];
        }
        last[8] = new_val;
    }
    return new_val;
}
