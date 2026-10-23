/*
 * A100978  Number of all extensions over Q_5 with degree n in the algebraic
 *          closure of Q_5.
 * https://oeis.org/A100978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{d|h} d) * (Sum_{s=0..m} (p^(m+s+1)-p^(2*s))/(p-1) *
 *            (p^(eps(s)*n)-p^(eps(s-1)*n))) , for p = 5, n = h * 5^m,
 *            (h,5)=1, eps(s) = Sum_{i=1..s} 1/5^i, eps(-1) = -∞
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 49, u64 n <= 99, u128 n <= 224,
 *            big any n
 * Helpers:   pow_seq, sigma1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"   /* pow_seq(seq_t base, idx_t exp) */
#include "../utils/sigma1.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset starts at 1, but guard against n=0 */

    /* Factor out powers of 5: n = h * 5^m with (h,5)=1. */
    idx_t m = 0;
    idx_t tmp = n;
    while ((tmp % 5) == 0) {
        tmp /= 5;
        ++m;
    }
    idx_t h = tmp;              /* coprime part */

    const seq_t P5 = (seq_t)5;
    const seq_t FOUR = (seq_t)4;

    /* Sum of divisors of h. */
    seq_t sigma = sigma1(h);

    /* Sum over s = 0..m of the inner terms. */
    seq_t sum_B = 0;

    /* s = 0 case */
    {
        seq_t factor = (pow_seq(P5, m + 1) - (seq_t)1) / FOUR; /* (p^(m+1)-1)/(p-1) */
        sum_B += factor; /* diff = 1 for s=0 */
    }

    /* For s >= 1 */
    seq_t G_prev = (seq_t)1;           /* G_0 = 1 */
    idx_t pi = 1;                      /* pi = 5^s; starts at 1 for s=0 */
    for (idx_t s = 1; s <= m; ++s) {
        pi *= (idx_t)5;                /* pi = 5^s */
        idx_t e = n / pi;              /* integer division since s <= m */
        seq_t term = pow_seq(P5, e);   /* 5^e */
        seq_t G_curr = G_prev * term;  /* G_s = G_{s-1} * 5^e */

        seq_t diff = G_curr - G_prev;  /* p^(eps(s)*n)-p^(eps(s-1)*n) */

        seq_t numerator = pow_seq(P5, m + s + 1) - pow_seq(P5, 2 * s);
        seq_t factor = numerator / FOUR; /* (p^(m+s+1)-p^(2s))/(p-1) */
        sum_B += factor * diff;

        G_prev = G_curr;
    }

    return sigma * sum_B;
}
