/*
 * A054058  Inverse Moebius transform of A000031 (starting at term 0).
 * https://oeis.org/A054058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{ d divides n+1 } A000031(d-1), with A000031(n) =
 *            (1/n)*Sum_{ e divides n } phi(e)*2^(n/e), A000031(0) = 1
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 34 (2026-11-07)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

static seq_t A000031_pow2_divmod(unsigned long long k, unsigned long long m, unsigned long long *rem)
{
    seq_t q = (seq_t)(1ULL / m);
    unsigned long long r = 1ULL % m;
    unsigned long long i;
    for (i = 0; i < k; i++) {
        q = q + q;
        r = r + r;
        if (r >= m) {
            r -= m;
            q += 1;
        }
    }
    *rem = r;
    return q;
}

static seq_t A000031_a(idx_t n)
{
    unsigned long long nn, d, R = 0;
    seq_t Q = 0;
    if (n == 0)
        return 1;
    nn = (unsigned long long)n;
    for (d = 1; d <= nn; d++) {
        unsigned long long r, ph, t;
        seq_t q;
        if (nn % d != 0)
            continue;
        ph = totient_u64(d);
        q = A000031_pow2_divmod(nn / d, nn, &r);
        Q += q * (seq_t)ph;
        t = ph * r;
        Q += (seq_t)(t / nn);
        R += t % nn;
        if (R >= nn) {
            R -= nn;
            Q += 1;
        }
    }
    return Q;
}

seq_t a(idx_t n)
{
    unsigned long long m = (unsigned long long)n + 1ULL;
    unsigned long long d;
    seq_t s = 0;
    for (d = 1; d <= m; d++) {
        if (m % d != 0)
            continue;
        s += A000031_a((idx_t)(d - 1));
    }
    return s;
}
