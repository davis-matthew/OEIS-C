/*
 * A025533  a(n) = (1/C(n,0) + 1/C(n,1) + ... + 1/C(n,n))*L, where L =
 *          LCM{C(n,0), C(n,1),..., C(n,n)}.
 * https://oeis.org/A025533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∑_{k=0}^{n} 1/C(n,k)) * L, where L = LCM{C(n,0), C(n,1),
 *            …, C(n,n)} and C(n,k) = binomial coefficient
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation follows the known identity */
 /* L = lcm(1,2,…,n+1) / (n+1)   */
 /* a(n) = Σ_{k=0}^{n} L / C(n,k)   */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* 1. Compute L = lcm(1,…,n+1) / (n+1) */
    seq_t l = 1;
    for (idx_t i = 1; i <= n + 1; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t g = gcd(l, i_seq);
        l = (l / g) * i_seq;
    }
    l = l / (seq_t)(n + 1);

    /* 2. Sum L / C(n,k) for k = 0…n */
    seq_t sum = 0;
    seq_t c   = 1;          /* c = C(n,0) initially */
    seq_t n_seq = (seq_t)n;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = l / c;           /* exact division */
        sum += term;

        if (k == n) break;            /* avoid last update */

        seq_t num = n_seq - (seq_t)k;  /* n - k   */
        seq_t den = (seq_t)(k + 1);    /* k + 1   */
        c = (c * num) / den;          /* next binomial */
    }

    return sum;
}
