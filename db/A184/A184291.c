/*
 * A184291  Table read by antidiagonals: T(n,k) = number of distinct n X k
 *          toroidal 0..5 arrays.
 * https://oeis.org/A184291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry of the table T(n,k) read by *
 *            antidiagonals, where * T(n,k) = (1/(n*k)) * Σ_{c|n} Σ_{d|k} φ(c)
 *            φ(d) 6^{n*k / lcm(c,d)}, * with n,k ≥ 1. The mapping from the
 *            sequence index n to (n,k) is * let s be the smallest integer
 *            with s(s+1)/2 ≥ n; * let prev = (s-1)s/2, * offset = n -...
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"      /* seq_t phi(idx_t m); */
#include "../utils/big_pow.h"  /* seq_t big_pow(seq_t base, seq_t exp); */

static inline idx_t gcd_idx_t(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range guard */

    /* Find the antidiagonal s containing the n‑th term */
    idx_t s = 1;
    idx_t tri = 1;                 /* s*(s+1)/2 */
    while (tri < n) {
        ++s;
        tri = s * (s + 1) / 2;
    }

    idx_t prev   = (s - 1) * s / 2;    /* terms before this diagonal */
    idx_t offset = n - prev - 1;       /* 0‑based position on diagonal */

    idx_t i = offset + 1;              /* row index n in T(n,k) */
    idx_t j = s - i + 1;               /* column index k in T(n,k) */

    /* Compute T(i,j) */
    seq_t sum = 0;

    for (idx_t c = 1; c <= i; ++c) {
        if (i % c != 0) continue;
        seq_t phi_c = phi(c);

        for (idx_t d = 1; d <= j; ++d) {
            if (j % d != 0) continue;
            seq_t phi_d = phi(d);

            idx_t g   = gcd_idx_t(c, d);
            idx_t lcm = (c / g) * d;
            idx_t exp = (i * j) / lcm;    /* exponent for 6 */

            seq_t pow6 = big_pow((seq_t)6, (seq_t)exp);
            sum += phi_c * phi_d * pow6;
        }
    }

    seq_t denom = (seq_t)i * (seq_t)j;
    return sum / denom;
}
