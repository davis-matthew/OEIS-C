/*
 * A205628  Number of 4X(n+1) 0..2 arrays with every 2X2 subblock in a row
 *          having an equal number of equal diagonal or equal antidiagonal
 *          elements, adjacent rows differing in this number, and new values
 *          0..2 introduced in row major order.
 * https://oeis.org/A205628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 23*a(n-1) -174*a(n-2) +284*a(n-3) +2352*a(n-4)
 *            -10052*a(n-5) -1168*a(n-6) +65760*a(n-7) -78080*a(n-8)
 *            -116928*a(n-9) +255232*a(n-10) -49664*a(n-11) -116736*a(n-12)
 *            +49152*a(n-13) Initial values for n=1..13: 396, 2052, 13068,
 *            94116, 708300, 5439324, 42094188, 327077484, 2547102732,
 *            19867584...
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const idx_t init_vals[13] = {
    396ULL, 2052ULL, 13068ULL, 94116ULL, 708300ULL, 5439324ULL,
    42094188ULL, 327077484ULL, 2547102732ULL, 19867584972ULL,
    155171240076ULL, 1213352429196ULL, 9498114849804ULL
};

static const int coeff[13] = {
    23,   -174,  284, 2352,   -10052, -1168, 65760,
    -78080, -116928, 255232, -49664, -116736, 49152
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* value for index 0 is unused */
    if (n <= 13) return init_vals[n - 1];

    seq_t prev[13];
    for (size_t i = 0; i < 13; ++i) prev[i] = init_vals[i];

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    seq_t prod, term;
    unsigned m;

    for (idx_t cur = 14; cur <= n; ++cur) {
        sum_pos = 0;
        sum_neg = 0;
        for (int i = 0; i < 13; ++i) {
            seq_t a_val = prev[12 - i];
            int c = coeff[i];
            if (c >= 0) {
                m = (unsigned) c;
                prod = a_val * (seq_t) m;
                sum_pos += prod;
            } else {
                m = (unsigned) -c;
                prod = a_val * (seq_t) m;
                sum_neg += prod;
            }
        }
        term = sum_pos;
        term -= sum_neg;           /* since recurrence guarantees term >= 0 */

        /* shift the ring buffer */
        for (int i = 0; i < 12; ++i) prev[i] = prev[i + 1];
        prev[12] = term;
    }

    return prev[12];
}
