/*
 * A143768  E.g.f. satisfies A(x) = exp(x + x^2*A(x)^2).
 * https://oeis.org/A143768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} C(k,n-k) * (2*(n-k)+1)^(k-1)/k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Implementation uses the equivalent form with r = n‑k) */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t sum = 0;
    seq_t f = 1;                     /* f = n! / (0! * n!) initially */
    idx_t limit = n / 2;             /* floor(n/2) */
    for (idx_t r = 0; r <= limit; ++r) {
        seq_t pow_term = big_pow((seq_t)(2 * r + 1), (seq_t)(n - r - 1));
        seq_t term = f * pow_term;
        sum += term;
        if (r == limit) break;       /* no further update needed */
        seq_t num1 = (seq_t)(n - 2 * r);
        seq_t num2 = (seq_t)(n - 2 * r - 1);
        seq_t denom = (seq_t)(r + 1);
        f = f * num1;
        f = f * num2;
        f = f / denom;               /* exact division */
    }
    return sum;
}
