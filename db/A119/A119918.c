/*
 * A119918  Table read by antidiagonals: number of rationals in [0, 1) having
 *          exactly n preperiodic bits, then exactly k periodic bits (read up
 *          antidiagonals).
 * https://oeis.org/A119918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, tabl
 * Formula:   a(n) is the n‑th element of the anti‑diagonal reading of the
 *            table * T(i,j) = 2^max(0,i-1) * Σ_{d | j} (2^d - 1) μ(j/d), *
 *            where i ≥ 0 counts preperiodic bits and j ≥ 1 counts periodic
 *            bits. * The linear index n (n ≥ 1) is obtained by ordering the
 *            pairs * by the sum i+j increasing and, within eac...
 * Range:     u8 n <= 44, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>            // for idx_t, seq_t
#include "../utils/pow2.h"     // seq_t pow2(idx_t)

static inline int mu_of(idx_t n)
{
    if (n == 1) return 1;
    int cnt = 0;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            ++cnt;
            t /= p;
            if (t % p == 0) return 0;      /* square factor */
            while (t % p == 0) t /= p;     /* skip further powers */
        }
    }
    if (t > 1) ++cnt;                         /* remaining prime factor */
    return (cnt & 1) ? -1 : 1;                /* (-1)^cnt  */
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n<1 */

    /* locate diagonal s such that s*(s+1)/2 >= n */
    idx_t s = 1;
    while ((s * (s + 1)) / 2 < n) ++s;

    idx_t prev = ((s - 1) * s) / 2;          /* elements before diag s */
    idx_t offset = n - prev - 1;             /* 0‑based inside diag s */
    idx_t i = (s - 1) - offset;              /* preperiodic bits (i≥0) */
    idx_t j = s - i;                         /* periodic bits   (j≥1)  */

    seq_t front = pow2((i > 0) ? (i - 1) : 0);/* 2^max(0,i-1)    */

    /* compute Σ_{d | j} (2^d - 1) μ(j/d) */
    seq_t pos = 0, neg = 0;
    for (idx_t d = 1; d <= j; ++d) {
        if (j % d == 0) {
            seq_t term = pow2(d);
            term -= 1;                       /* 2^d - 1          */
            int mu = mu_of(j / d);
            if (mu ==  1) pos += term;
            else if (mu == -1) neg += term;
        }
    }
    seq_t diff = pos;
    diff -= neg;                             /* pos ≥ neg for valid j */

    return front * diff;
}
