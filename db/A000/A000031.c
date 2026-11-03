/*
 * A000031  Number of n-bead necklaces with 2 colors when turning over is not
 *          allowed; also number of output sequences from a simple n-stage
 *          cycling shift register; also number of binary irreducible
 *          polynomials whose degree divides n.
 * https://oeis.org/A000031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   a(n) = (1/n)*Sum_{ d divides n } phi(d)*2^(n/d), a(0) = 1
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 70, u128 n <= 135,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 3197 (2026-11-01)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

/* floor(2^k / m) as seq_t, with remainder in *rem (computed by long division) */
static seq_t pow2_divmod(unsigned long long k, unsigned long long m, unsigned long long *rem)
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

seq_t a(idx_t n)
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
        q = pow2_divmod(nn / d, nn, &r);
        /* phi(d) * 2^(n/d) = phi(d)*q*n + phi(d)*r */
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
