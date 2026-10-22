/*
 * A003245  Nearest integer to -4n/Bernoulli(2n).
 * https://oeis.org/A003245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = round(-4n/B(2n)), a(0) = 0, where B(m) are the Bernoulli
 *            numbers: B(0) = 1, B(m) = -1/(m+1) * Sum_{k=0..m-1}
 *            binomial(m+1,k)*B(k). For n >= 3, |B(2n+2)/B(2n)| =
 *            (2n+2)(2n+1)*zeta(2n+2)/(4*Pi^2*zeta(2n)) > (n+1)/n, so
 *            |4n/B(2n)| decreases. It is already < 1/2 at n = 10
 *            (40*330/174611 ~ 0.07...
 * Range:     i8 n <= 1, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   a3245_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/a3245_gcd.h"

#define A3245_MAXM 20

static __int128 a3245_num[A3245_MAXM + 1];
static __int128 a3245_den[A3245_MAXM + 1];
static bool a3245_ready = false;

static void a3245_init(void) {
    int m, k;
    a3245_num[0] = 1;
    a3245_den[0] = 1;
    for (m = 1; m <= A3245_MAXM; m++) {
        __int128 sn = 0, sd = 1, c = 1, g;
        for (k = 0; k < m; k++) {
            /* c = binomial(m+1, k) */
            __int128 tn = c * a3245_num[k];
            __int128 td = a3245_den[k];
            sn = sn * td + tn * sd;
            sd = sd * td;
            g = a3245_gcd(sn, sd);
            if (g > 1) { sn /= g; sd /= g; }
            c = c * (m + 1 - k) / (k + 1);
        }
        sn = -sn;
        sd = sd * (m + 1);
        if (sd < 0) { sd = -sd; sn = -sn; }
        g = a3245_gcd(sn, sd);
        if (g > 1) { sn /= g; sd /= g; }
        if (sn == 0) sd = 1;
        a3245_num[m] = sn;
        a3245_den[m] = sd;
    }
    a3245_ready = true;
}

seq_t a(idx_t n) {
    __int128 N, D, q;
    bool neg;
    if (n == 0) return 0;
    if (n >= 10) return 0;
    if (!a3245_ready) a3245_init();
    /* -4n / (num/den) = -4n*den / num */
    N = -(__int128)4 * (__int128)n * a3245_den[2 * n];
    D = a3245_num[2 * n];
    neg = false;
    if (N < 0) { N = -N; neg = !neg; }
    if (D < 0) { D = -D; neg = !neg; }
    q = (2 * N + D) / (2 * D);
    if (neg) q = -q;
    return (seq_t)(long long)q;
}
