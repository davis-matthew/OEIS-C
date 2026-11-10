/*
 * A054156  Moebius transform of A000029 (starting at term 0).
 * https://oeis.org/A054156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|(n+1)} mu((n+1)/d) * A000029(d-1), with A000029(k)
 *            = Sum_{d|k} phi(d)*2^(k/d)/(2k) + (2^((k-1)/2) if k odd, else
 *            2^(k/2-1)+2^(k/2-2)), A000029(0)=1
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-07)
 * Helpers:   mobius_u64, pow2_u64, totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2_u64.h"
#include "../utils/totient_u64.h"
#include "../utils/mobius_u64.h"

static seq_t A000029_a(idx_t n)
{
    unsigned long long M, R, d;
    seq_t Q, half;

    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 3;

    M = 2ULL * (unsigned long long)n;
    Q = 0;
    R = 0;
    for (d = 1; d <= (unsigned long long)n; d++) {
        unsigned long long m, i, r, ph;
        seq_t q;
        if ((unsigned long long)n % d != 0) continue;
        m = (unsigned long long)n / d;
        q = 0;
        r = 1;
        for (i = 0; i < m; i++) {
            q += q;
            r += r;
            if (r >= M) {
                r -= M;
                q += 1;
            }
        }
        ph = totient_u64(d);
        Q += q * (seq_t)ph;
        R += r * ph;
        Q += (seq_t)(R / M);
        R %= M;
    }

    if (n & 1)
        half = pow2_u64(((unsigned long long)n - 1) / 2);
    else
        half = pow2_u64((unsigned long long)n / 2 - 2) * 3;

    return Q + half;
}

seq_t a(idx_t n)
{
    unsigned long long m = (unsigned long long)n + 1ULL;
    unsigned long long d;
    seq_t pos = 0, neg = 0;

    for (d = 1; d <= m; d++) {
        int mu;
        if (m % d != 0) continue;
        mu = mobius_u64(m / d);
        if (mu == 0) continue;
        if (mu > 0) pos += A000029_a((idx_t)(d - 1));
        else        neg += A000029_a((idx_t)(d - 1));
    }
    return pos - neg;
}
