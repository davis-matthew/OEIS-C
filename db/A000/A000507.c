/*
 * A000507  Number of permutations of [n] with exactly 3 increasing runs of
 *          length at least 2.
 * https://oeis.org/A000507
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: x^6*(61 - 445*x + 963*x^2 - 675*x^3)/((1 - 5*x)^2*(1 -
 *            x)^4*(1 - 3*x)^3*(1 - 7*x)), evaluated as the order-10 linear
 *            recurrence given by the denominator, with a(6) = 61 and terms
 *            below the offset taken as 0
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const long long roots[10] = {5, 5, 1, 1, 1, 1, 3, 3, 3, 7};
    static const long long num[4] = {61, -445, 963, -675};
    long long d[11];
    int deg = 0, i, j;
    seq_t w[11];
    idx_t m, k;

    for (i = 0; i < 11; i++) d[i] = 0;
    d[0] = 1;
    for (i = 0; i < 10; i++) {
        for (j = deg + 1; j >= 1; j--) d[j] -= roots[i] * d[j - 1];
        deg++;
    }

    m = n - 6;
    for (k = 0; k <= m; k++) {
        seq_t pos = 0, neg = 0;
        if (k < 4) {
            long long c = num[k];
            if (c > 0) pos += (seq_t)c;
            else neg += (seq_t)(-c);
        }
        for (i = 1; i <= 10; i++) {
            long long c;
            if (k < (idx_t)i) break;
            c = -d[i];
            if (c > 0) pos += w[(k - i) % 11] * (seq_t)c;
            else if (c < 0) neg += w[(k - i) % 11] * (seq_t)(-c);
        }
        w[k % 11] = pos - neg;
    }
    return w[m % 11];
}
