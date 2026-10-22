/*
 * A007148  Number of self-complementary 2-colored bracelets (turnover
 *          necklaces) with 2n beads.
 * https://oeis.org/A007148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2^(n-2) + (1/(4n)) * Sum_{d|n} phi(2d)*2^(n/d)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw2.h"

static unsigned long long totient(unsigned long long n) {
    unsigned long long result = n;
    for (unsigned long long p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

/* Add phi(2d)*2^e to (*Q)*M + *R, keeping *R < M, with no overflowing
   intermediate values. */
static void add_term(seq_t *Q, unsigned long long *R, unsigned long long M,
                     unsigned long long c, idx_t e) {
    seq_t q = 0;
    unsigned long long r = 1 % M;
    if (M == 1) q = 1;
    for (idx_t i = 0; i < e; i++) {
        q = q + q;
        r = r * 2;
        if (r >= M) { r -= M; q += 1; }
    }
    /* multiply (q, r) by c */
    unsigned long long rc = r * c;
    q = q * (seq_t)c + (seq_t)(rc / M);
    r = rc % M;
    *Q += q;
    *R += r;
    if (*R >= M) { *Q += (seq_t)(*R / M); *R %= M; }
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    unsigned long long M = 4ULL * (unsigned long long)n;
    seq_t Q = 0;
    unsigned long long R = 0;
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            add_term(&Q, &R, M, totient(2 * d), n / d);
            idx_t d2 = n / d;
            if (d2 != d)
                add_term(&Q, &R, M, totient(2 * d2), n / d2);
        }
    }
    return pw2(n - 2) + Q + (seq_t)(R / M);
}
