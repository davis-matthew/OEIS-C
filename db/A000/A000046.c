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
 * Helpers:   divpow2, mobius_ull, pow2_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2_s.h"
#include "../utils/divpow2.h"
#include "../utils/mobius_ull.h"

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
            if (mu > 0) spos += pow2_s(d / 2);
            else if (mu < 0) sneg += pow2_s(d / 2);
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
