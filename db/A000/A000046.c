/*
 * A000046  Number of primitive n-bead necklaces (turning over is allowed)
 *          where complements are equivalent.
 * https://oeis.org/A000046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = ((1/(2n))*Sum_{odd d|n} mu(d)*2^(n/d) + Sum_{d|n}
 *            mu(n/d)*2^floor(d/2)) / 2, a(0) = 1
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 39, u64 n <= 72, u128 n <= 137,
 *            big any n
 * Validated: standard, pass, 57 terms through n = 972 (2026-11-01)
 * Helpers:   pow2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2_u64.h"

/* floor(2^k / m), remainder in *rem, by binary long division */
static seq_t divpow2(unsigned long long k, unsigned long long m, unsigned long long *rem)
{
    seq_t q = 0;
    unsigned long long r = 1, i;
    if (r >= m) { r -= m; q = 1; }
    for (i = 0; i < k; i++) {
        r <<= 1;
        q += q;
        if (r >= m) { r -= m; q += 1; }
    }
    *rem = r;
    return q;
}

static int mobius_ull(unsigned long long x)
{
    int m = 1;
    unsigned long long p;
    for (p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            x /= p;
            if (x % p == 0) return 0;
            m = -m;
        }
    }
    if (x > 1) m = -m;
    return m;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    unsigned long long N = (unsigned long long)n;
    unsigned long long m4 = 4ULL * N;
    seq_t qpos = 0, qneg = 0, spos = 0, sneg = 0;
    long long R = 0;
    unsigned long long d;
    for (d = 1; d <= N; d++) {
        if (N % d) continue;
        if (d & 1) {
            int mu = mobius_ull(d);
            if (mu != 0) {
                unsigned long long r;
                seq_t q = divpow2(N / d, m4, &r);
                if (mu > 0) { qpos += q; R += (long long)r; }
                else { qneg += q; R -= (long long)r; }
            }
        }
        {
            int mu = mobius_ull(N / d);
            if (mu > 0) spos += pow2_u64(d / 2);
            else if (mu < 0) sneg += pow2_u64(d / 2);
        }
    }
    long long c = R / (long long)(2ULL * N);
    seq_t s = spos - sneg;
    long long odd = (long long)(s & 1);
    seq_t s_half = s / 2;
    long long ch = (c + odd) / 2;
    seq_t res = qpos + s_half;
    if (ch > 0) res += (seq_t)(unsigned long long)ch;
    res -= qneg;
    if (ch < 0) res -= (seq_t)(unsigned long long)(-ch);
    return res;
}
