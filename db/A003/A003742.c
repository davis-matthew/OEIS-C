/*
 * A003742  Number of 2-factors in O_5 X P_n.
 * https://oeis.org/A003742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26a(n-1) + 396a(n-2) - 707a(n-3) - 6539a(n-4) +
 *            7239a(n-5) - 405a(n-6), a(1) = 6, a(2) = 327, a(3) = 11040, a(4)
 *            = 406731, a(5) = 14683587, a(6) = 532938234
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   mk3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mk3.h"

seq_t a(idx_t n) {
    seq_t v[6];
    seq_t pos, neg, nx;
    idx_t i;

    v[0] = mk3(0, 0, 6);          /* a(1) */
    v[1] = mk3(0, 0, 327);        /* a(2) */
    v[2] = mk3(0, 11, 40);        /* a(3) */
    v[3] = mk3(0, 406, 731);      /* a(4) */
    v[4] = mk3(14, 683, 587);     /* a(5) */
    v[5] = mk3(532, 938, 234);    /* a(6) */

    if (n <= 6) return v[n - 1];

    /* v[5] = a(i-1), v[4] = a(i-2), ..., v[0] = a(i-6) */
    for (i = 7; i <= n; i++) {
        pos = v[5] * 26 + v[4] * 396 + v[1] * 7239;
        neg = v[3] * 707 + v[2] * 6539 + v[0] * 405;
        nx = pos - neg;
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = nx;
    }
    return v[5];
}
