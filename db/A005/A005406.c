/*
 * A005406  Number of protruded partitions of n with largest part at most 5.
 * https://oeis.org/A005406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: (1-x)^5/Product(1-x-x^i+x^(1+2*i), i=1..5)-1; a(n) = [x^n]
 *            of that series, computed by the linear recurrence given by the
 *            denominator (degree 35)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

#define DEG 35
#define RING 64

seq_t a(idx_t n) {
    long long d[DEG + 1];
    long long p[12];
    long long nd[DEG + 1];
    int deg = 0, i, j, k;
    static const int num[6] = {1, -5, 10, -10, 5, -1};
    seq_t h[RING];
    idx_t m;

    if (n == 0) return 0;

    for (i = 0; i <= DEG; i++) d[i] = 0;
    d[0] = 1;
    for (k = 1; k <= 5; k++) {
        int pd = 2 * k + 1;
        for (j = 0; j < 12; j++) p[j] = 0;
        p[0] += 1;
        p[1] -= 1;
        p[k] -= 1;
        p[pd] += 1;
        for (i = 0; i <= DEG; i++) nd[i] = 0;
        for (i = 0; i <= deg; i++)
            for (j = 0; j <= pd; j++)
                if (i + j <= DEG) nd[i + j] += d[i] * p[j];
        for (i = 0; i <= DEG; i++) d[i] = nd[i];
        deg += pd;
    }

    for (i = 0; i < RING; i++) h[i] = 0;
    h[0] = 1;

    for (m = 1; m <= n; m++) {
        seq_t pos = 0, neg = 0;
        long long c = (m < 6) ? num[m] : 0;
        if (c > 0) pos += (idx_t)c;
        else if (c < 0) neg += (idx_t)(-c);
        for (j = 1; j <= DEG && (idx_t)j <= m; j++) {
            long long dj = d[j];
            if (dj == 0) continue;
            {
                seq_t hv = h[(m - (idx_t)j) % RING];
                if (dj < 0) pos += hv * (idx_t)(-dj);
                else neg += hv * (idx_t)dj;
            }
        }
        h[m % RING] = pos - neg;
    }

    return h[n % RING];
}
