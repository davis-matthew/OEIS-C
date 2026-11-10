/*
 * A054192  Binomial transform of A000029.
 * https://oeis.org/A054192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*A000029(k)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 895 (2026-11-07)
 * Helpers:   pow2_u64, totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2_u64.h"
#include "../utils/totient_u64.h"

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
    seq_t C = 1, sum = 0;
    idx_t k;
    for (k = 0; k <= n; k++) {
        sum += C * A000029_a(k);
        if (k < n) {
            C = C * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }
    return sum;
}
