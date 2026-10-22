/*
 * A003402  G.f.: 1/((1-x)*(1-x^2)*(1-x^3)^2*(1-x^4)*(1-x^5)).
 * https://oeis.org/A003402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. 1/((1-x)*(1-x^2)*(1-x^3)^2*(1-x^4)*(1-x^5)); with D(x) =
 *            sum_{k=0..18} d_k x^k the denominator, a(n) = -Sum_{k=1..18}
 *            d_k*a(n-k) for n >= 1, a(0) = 1, a(n) = 0 for n < 0
 * Range:     u8 n <= 17, u16 n <= 68, u32 n <= 704, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    long long d[19];
    int deg = 0;
    int parts[6] = {1, 2, 3, 3, 4, 5};
    int i, j, p;
    for (i = 0; i < 19; i++) d[i] = 0;
    d[0] = 1;
    for (p = 0; p < 6; p++) {
        int s = parts[p];
        for (j = deg + s; j >= s; j--) d[j] -= d[j - s];
        deg += s;
    }

    seq_t buf[19];
    for (i = 0; i < 19; i++) buf[i] = 0;

    seq_t last = 1;
    for (idx_t m = 0; m <= n; m++) {
        seq_t pos = 0, neg = 0;
        if (m == 0) pos = 1;
        for (int k = 1; k <= 18 && (idx_t)k <= m; k++) {
            long long c = -d[k];
            const seq_t prev = buf[(m - (idx_t)k) % 19];
            if (c > 0) {
                for (long long t = 0; t < c; t++) pos += prev;
            } else if (c < 0) {
                for (long long t = 0; t < -c; t++) neg += prev;
            }
        }
        last = pos - neg;
        buf[m % 19] = last;
    }
    return last;
}
