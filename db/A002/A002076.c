/*
 * A002076  Number of equivalence classes of base-3 necklaces of length n,
 *          where necklaces are considered equivalent under both rotations and
 *          permutations of the symbols.
 * https://oeis.org/A002076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1; for n > 0, a(n) = (1/n) * Sum_{d|n} phi(d) *
 *            f_d(3^(n/d)), where f_d(x) = x if d==0 mod 6, (x+1)/2 if d==3
 *            mod 6, 2x/3 if d==2,4 mod 6, (x+3)/6 if d==1,5 mod 6
 *            (coefficient extraction from the g.f. 1 - Sum_{d>0} (phi(d)/d) *
 *            (...log(1-3x^d), log(1-x^d)...))
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 45, u128 n <= 86,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 441 (2026-11-01)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

/* Adds phi(d) * (alpha*3^k + beta) / (6n) to Q + R/(6n), with k = n/d. */
static void a2076_term(idx_t n, idx_t d, seq_t *Q, unsigned __int128 *R) {
    uint64_t M = 6 * (uint64_t)n;
    idx_t k = n / d;
    seq_t q = 0;
    uint64_t r = 1;
    idx_t i;
    uint64_t ph, alpha, beta;
    unsigned dm = (unsigned)(d % 6);

    for (i = 0; i < k; i++) {
        r *= 3;
        q = q * 3 + (seq_t)(r / M);
        r %= M;
    }
    /* now 3^k = q*M + r */
    if (dm == 0) { alpha = 6; beta = 0; }
    else if (dm == 3) { alpha = 3; beta = 3; }
    else if (dm == 2 || dm == 4) { alpha = 4; beta = 0; }
    else { alpha = 1; beta = 3; }

    ph = totient_u64((uint64_t)d);
    *Q += q * (seq_t)(ph * alpha);
    *R += (unsigned __int128)ph * (unsigned __int128)(alpha * r + beta);
    *Q += (seq_t)(uint64_t)(*R / M);
    *R %= M;
}

seq_t a(idx_t n) {
    seq_t Q = 0;
    unsigned __int128 R = 0;
    idx_t d;
    if (n == 0) return (seq_t)1;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            a2076_term(n, d, &Q, &R);
            if (d != n / d) a2076_term(n, n / d, &Q, &R);
        }
    }
    Q += (seq_t)(uint64_t)(R / (6 * (uint64_t)n));
    return Q;
}
