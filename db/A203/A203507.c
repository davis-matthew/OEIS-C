/*
 * A203507  G.f.: Product_{n>=0} 1/(1-a(n)*x^(n+1))^3 = Sum_{n>=0} a(n)*x^n.
 * https://oeis.org/A203507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the generating function \prod_{i\ge 0}
 *            1/(1-a(i)\,x^{\,i+1})^3 = \sum_{n\ge 0} a(n)\,x^n. Recurrence
 *            (derived from the exponential of the logarithm) a(0)=1, a(n) =
 *            (1/n) \sum_{k=1}^{n} K(k) * a(n-k) for n\ge 1 where K(k) =
 *            \sum_{d|k} 3*d * a(d-1)^{\,k/d}. The implementation memoise...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   seq_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* memoised arrays */
    static seq_t *aseq = NULL;   /* a[0..max] */
    static seq_t *karr = NULL;   /* K[0..max] */
    static idx_t  ccap  = 0;      /* capacity of the arrays          */
    static idx_t  done  = 0;      /* last index that has been computed */

    /* grow arrays if needed */
    if (n + 1 > ccap) {
        idx_t new_cap = n + 1;
        seq_t *tmp;
        tmp = seq_realloc(aseq, ccap, new_cap);
        aseq = tmp;
        tmp = seq_realloc(karr, ccap, new_cap);
        karr = tmp;
        ccap = new_cap;
    }

    /* initialise a(0) the first time */
    if (done == 0 && aseq[0] == 0)
        aseq[0] = 1;

    /* compute values up to n sequentially */
    for (idx_t i = done + 1; i <= n; ++i) {
        /* compute K(i) = sum_{d|i} 3*d * a(d-1)^{i/d} */
        seq_t Ki = 0;
        for (idx_t d = 1; d <= i; ++d) {
            if (i % d == 0) {
                idx_t exp = i / d;
                seq_t pow_val = seq_pow(aseq[d - 1], exp);
                seq_t term = pow_val * (((seq_t)3) * d);
                Ki += term;
            }
        }
        karr[i] = Ki;

        /* a(i) = (1/i) * sum_{k=1}^{i} K(k) * a(i-k) */
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k)
            sum += karr[k] * aseq[i - k];
        aseq[i] = sum / i;          /* division is exact */
    }

    done = (n > done) ? n : done;
    return aseq[n];
}
