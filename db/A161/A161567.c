/*
 * A161567  E.g.f. satisfies: A(x) = exp(x*exp(x*A(x)^2)).
 * https://oeis.org/A161567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} C(n,k)·(2·(n−k)+1)^{k−1} · k^{\,n−k}
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer‐exponentiation of arbitrary precision integer type */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum   = (seq_t)0;
    seq_t binom = (seq_t)1;           /* C(n,0) */

    for (idx_t k = 1; k <= n; ++k) {
        /* binom = C(n,k) */
        binom = binom * (seq_t)(n - k + 1);
        binom = binom / (seq_t)k;

        seq_t base1 = (seq_t)2 * (seq_t)(n - k) + (seq_t)1;
        seq_t pow1  = pow_seq(base1, k - 1);

        seq_t pow2  = pow_seq((seq_t)k, n - k);

        seq_t term = binom;
        term = term * pow1;
        term = term * pow2;

        sum = sum + term;
    }
    return sum;
}
