/*
 * A005687  Number of Twopins positions.
 * https://oeis.org/A005687
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: x^7/((1-x^2-x^5)*(1-2*x+x^2-x^5)), i.e. a(n) = 2*a(n-1) -
 *            2*a(n-3) + a(n-4) + 2*a(n-5) - 2*a(n-6) - a(n-10), with a(7) = 1
 *            and a(n) = 0 for n < 7
 * Range:     u8 n <= 18, u16 n <= 31, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t h[10];
    idx_t m, k;
    int j;
    if (n < 7) return 0;
    m = n - 7;
    for (j = 0; j < 10; j++) h[j] = 0;
    for (k = 0; k <= m; k++) {
        seq_t pos = 2 * h[0] + h[3] + 2 * h[4];
        seq_t neg = 2 * h[2] + 2 * h[5] + h[9];
        seq_t v;
        if (k == 0) pos += 1;
        v = pos - neg;
        for (j = 9; j > 0; j--) h[j] = h[j - 1];
        h[0] = v;
    }
    return h[0];
}
