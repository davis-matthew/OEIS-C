/*
 * A002653  Expansion of (theta_3(z)*theta_3(7z)+theta_2(z)*theta_2(7z))^3.
 * https://oeis.org/A002653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   G.f. = theta(q)^3 where theta(q) = Sum_{x,y} q^(x^2+x*y+2*y^2);
 *            a(n) = Sum_{i+j+k=n} r(i)*r(j)*r(k), r(m) = #{(x,y):
 *            x^2+x*y+2*y^2 = m}
 * Range:     u8 n <= 5, u16 n <= 93, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    long long N = (long long)n;
    long long *r = (long long *)calloc((size_t)N + 1, sizeof(long long));
    long long *s = (long long *)calloc((size_t)N + 1, sizeof(long long));
    long long B = 0;
    while ((B + 1) * (B + 1) <= 4 * N) B++;
    /* 4*(x^2+xy+2y^2) = (2x+y)^2 + 7y^2 */
    for (long long y = -B; y <= B; y++) {
        if (7 * y * y > 4 * N) continue;
        for (long long x = -B - (y < 0 ? -y : y); x <= B + (y < 0 ? -y : y); x++) {
            long long v = x * x + x * y + 2 * y * y;
            if (v <= N) r[v]++;
        }
    }
    for (long long i = 0; i <= N; i++) {
        if (!r[i]) continue;
        for (long long j = 0; i + j <= N; j++)
            s[i + j] += r[i] * r[j];
    }
    seq_t ans = 0;
    for (long long i = 0; i <= N; i++) {
        if (r[i] && s[N - i]) ans += (seq_t)r[i] * (seq_t)s[N - i];
    }
    free(r);
    free(s);
    return ans;
}
