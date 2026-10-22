/*
 * A001037  Number of degree-n irreducible polynomials over GF(2); number of
 *          n-bead necklaces with beads of 2 colors when turning over is not
 *          allowed and with primitive period n; number of binary Lyndon words
 *          of length n.
 * https://oeis.org/A001037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice, changed
 * Formula:   a(n) = (1/n)*Sum_{d | n} mu(n/d)*2^d for n >= 1, a(0) = 1
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 70, u128 n <= 135,
 *            big any n
 * Helpers:   a1037_mu, a1037_qdiv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1037_mu.h"
#include "../utils/a1037_qdiv.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t pos = 0, neg = 0;
    long long R = 0;
    for (idx_t d = 1; d <= n; d++) {
        if (n % d != 0) continue;
        int mu = a1037_mu(n / d);
        if (mu == 0) continue;
        uint64_t r;
        seq_t q = a1037_qdiv(d, n, &r);
        if (mu > 0) {
            pos = pos + q;
            R += (long long)r;
        } else {
            neg = neg + q;
            R -= (long long)r;
        }
    }
    seq_t res = pos - neg;
    long long c = R / (long long)n;
    if (c >= 0) res = res + (seq_t)(unsigned long long)c;
    else res = res - (seq_t)(unsigned long long)(-c);
    return res;
}
