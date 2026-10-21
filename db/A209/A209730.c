/*
 * A209730  1/4 the number of (n+1)X3 0..3 arrays with every 2X2 subblock
 *          having distinct edge sums.
 * https://oeis.org/A209730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=124, a(2)=1096, a(3)=9712, a(4)=86744, a(5)=775096,
 *            a(6)=6933120, a(7)=62007896, a(8)=554698328, a(9)=4961776976,
 *            a(10)=44385441816, a(11)=397039390488 Recurrence (order 11):
 *            a(n) = 11*a(n-1) + 13*a(n-2) - 364*a(n-3) + 519*a(n-4) +
 *            2741*a(n-5) - 6099*a(n-6) - 4620*a(n-7) + 15632*a(n-8) - 169...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[11] = {
        (seq_t)124ULL,
        (seq_t)1096ULL,
        (seq_t)9712ULL,
        (seq_t)86744ULL,
        (seq_t)775096ULL,
        (seq_t)6933120ULL,
        (seq_t)62007896ULL,
        (seq_t)554698328ULL,
        (seq_t)4961776976ULL,
        (seq_t)44385441816ULL,
        (seq_t)397039390488ULL
    };
    if (n <= 11) {
        return init[n - 1];
    }

    static const int coeff[11] = {
        11, 13, -364, 519, 2741,
        -6099, -4620, 15632, -1696, -8896, 3328
    };

    /* buffer holds the 11 most recent terms in reverse order:
       buf[0] = a(k), buf[1] = a(k-1), ..., buf[10] = a(k-10)  */
    seq_t buf[11];
    buf[0] = init[10];
    buf[1] = init[9];
    buf[2] = init[8];
    buf[3] = init[7];
    buf[4] = init[6];
    buf[5] = init[5];
    buf[6] = init[4];
    buf[7] = init[3];
    buf[8] = init[2];
    buf[9] = init[1];
    buf[10] = init[0];

    for (idx_t cur = 12; cur <= n; ++cur) {
        seq_t sum_pos = 0, sum_neg = 0;
        for (int i = 0; i < 11; ++i) {
            if (coeff[i] > 0) {
                sum_pos += (seq_t)coeff[i] * buf[i];
            } else {
                sum_neg += (seq_t)(-coeff[i]) * buf[i];
            }
        }
        seq_t new_val = sum_pos - sum_neg; /* sum_pos >= sum_neg assured by the recurrence */

        /* shift buffer: make room for new_val */
        for (int i = 10; i > 0; --i) {
            buf[i] = buf[i - 1];
        }
        buf[0] = new_val;
    }

    return buf[0];
}
