/*
 * A003264  a(n) = floor((-4n)/Bernoulli(2n)).
 * https://oeis.org/A003264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = floor((-4n)/Bernoulli(2n)); B(m) from the recurrence B(0)
 *            = 1, B(m) = -1/(m+1) * Sum_{k=0..m-1} binomial(m+1,k)*B(k), as
 *            exact fractions. Once |B(2k)| > 4k, it stays so for all larger k
 *            (|B(2n)| grows superexponentially), so then a(n) = -1 for odd n
 *            (B(2n) > 0) and a(n) = 0 for even n (B(2...
 * Range:     i8 n <= 1, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   a3264_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a3264_gcd.h"

seq_t a(idx_t n) {
    __int128 N[128], D[128];
    long long row[130];
    int m, k;

    if (n == 0) return 0;  /* -0 / B(0) = 0 */

    N[0] = 1;
    D[0] = 1;
    /* row holds binomial(m+1, j) for j = 0..m+1 */
    row[0] = 1;
    row[1] = 1;
    for (m = 1; m < 120; m++) {
        __int128 sn = 0, sd = 1, g;
        /* advance row from binomial(m, .) to binomial(m+1, .) */
        row[m + 1] = 1;
        for (k = m; k >= 1; k--) row[k] = row[k] + row[k - 1];

        for (k = 0; k < m; k++) {
            if (N[k] == 0) continue;
            sn = sn * D[k] + (__int128)row[k] * N[k] * sd;
            sd = sd * D[k];
            g = a3264_gcd(sn, sd);
            if (g > 1) { sn /= g; sd /= g; }
        }
        N[m] = -sn;
        D[m] = sd * (m + 1);
        if (D[m] < 0) { D[m] = -D[m]; N[m] = -N[m]; }
        g = a3264_gcd(N[m], D[m]);
        if (g > 1) { N[m] /= g; D[m] /= g; }
        if (N[m] == 0) D[m] = 1;

        if (m % 2 == 0) {
            idx_t half = (idx_t)(m / 2);
            __int128 x = -(__int128)(4 * (long long)half) * D[m];
            __int128 y = N[m];
            if (half == n) {
                __int128 q = x / y;
                if (x % y != 0 && ((x < 0) != (y < 0))) q--;
                return (seq_t)(long long)q;
            }
            {
                __int128 absN = N[m] < 0 ? -N[m] : N[m];
                if (absN > (__int128)(4 * (long long)half) * D[m]) {
                    /* n > half here: |(-4n)/B(2n)| < 1 from now on */
                    return (n % 2 == 1) ? (seq_t)(-1) : (seq_t)0;
                }
            }
        }
    }
    return (n % 2 == 1) ? (seq_t)(-1) : (seq_t)0;
}
