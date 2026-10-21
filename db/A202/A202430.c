/*
 * A202430  Number of (n+2) X 5 binary arrays avoiding patterns 000 and 001 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +31*a(n-2) +70*a(n-3) -27*a(n-4) -200*a(n-5)
 *            +45*a(n-6) +376*a(n-7) -209*a(n-8) -395*a(n-9) +543*a(n-10)
 *            -207*a(n-11) -27*a(n-12) +27*a(n-13) +6*a(n-14) -3*a(n-15)
 *            -a(n-16) Initial values: a(1) = 1344, a(2) = 10080, a(3) =
 *            67072, a(4) = 463632, a(5) = 3166800, a(6) = 21708320, a(7)...
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[16] = {
    (seq_t)1344ULL,
    (seq_t)10080ULL,
    (seq_t)67072ULL,
    (seq_t)463632ULL,
    (seq_t)3166800ULL,
    (seq_t)21708320ULL,
    (seq_t)148654000ULL,
    (seq_t)1018262528ULL,
    (seq_t)6974320896ULL,
    (seq_t)47770334768ULL,
    (seq_t)327196846288ULL,
    (seq_t)2241105186080ULL,
    (seq_t)15350212751664ULL,
    (seq_t)105139754969120ULL,
    (seq_t)720144021240448ULL,
    (seq_t)4932553704580224ULL
};

static const int coeff[16] = {
    1, 31, 70, -27, -200, 45, 376, -209, -395, 543,
   -207, -27, 27, 6, -3, -1
};

seq_t a(idx_t n)
{
    /* a(1) ... a(16) are known */
    if (n <= 16) {
        return init[n - 1];
    }

    /* state holds the last 16 computed terms: state[0]=a(i-16), state[15]=a(i-1) */
    seq_t state[16];
    for (int i = 0; i < 16; ++i) {
        state[i] = init[i];
    }

    seq_t term = 0;
    for (idx_t i = 17; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int j = 0; j < 16; ++j) {
            int ck = coeff[j];
            /* state[15-j] corresponds to a(i-(j+1)) */
            if (ck >= 0) {
                sum_pos += (seq_t)ck * state[15 - j];
            } else {
                sum_neg += (seq_t)(-ck) * state[15 - j];
            }
        }
        term = sum_pos - sum_neg;

        /* shift the state: drop oldest, append the new term */
        for (int k = 0; k < 15; ++k) {
            state[k] = state[k + 1];
        }
        state[15] = term;
    }

    return term;
}
