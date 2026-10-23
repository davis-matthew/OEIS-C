/*
 * A189420  Expansion of e.g.f. exp(sin(x)+sin(x)^2).
 * https://oeis.org/A189420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n]exp(sin(x)+sin(x)^2) =e.g.f. expansion * Recurrence:
 *            a(0)=1, * for n≥0: a(n+1)=∑_{m=0}^{n}
 *            binomial(n,m)·a(m)·c_{n−m}, * where c_k = (k+1)·g_{k+1} with
 *            g(x)=sin(x)+sin^2(x). * Here c_k simplifies to: * if k even: c_k
 *            = (-1)^{k/2} * if k odd : c_k = (-1)^{(k−1)/2+2}·2^k
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate arrays */
    seq_t *a_seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!a_seq) return 0;      /* should not happen */
    a_seq[0] = 1;              /* a(0) = 1 */

    /* precompute powers of two up to n */
    seq_t *pow2 = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!pow2) { free(a_seq); return 0; }
    pow2[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow2[i] = pow2[i - 1] * 2;

    /* compute recursively */
    for (idx_t cur = 0; cur < n; ++cur) {
        seq_t sum = 0;
        seq_t binom = 1;   /* C(cur,0) */

        for (idx_t m = 0; m <= cur; ++m) {
            idx_t k = cur - m;          /* k = n - m */

            /* compute c_k */
            seq_t ck;
            if (k == 0) {
                ck = 1;
            } else if ((k & 1) == 0) {   /* k even */
                idx_t h = k >> 1;       /* k/2 */
                ck = (h & 1) ? -1 : 1;  /* (-1)^{k/2} */
            } else {                    /* k odd */
                idx_t h = (k - 1) >> 1;             /* (k-1)/2 */
                seq_t val = pow2[k];                 /* 2^k */
                if ((h + 2) & 1)                    /* (-1)^{(k-1)/2+2} */
                    val = -val;
                ck = val;
            }

            /* accumulate term: binom * a(m) * ck */
            seq_t prod = a_seq[m];
            prod = prod * binom;
            prod = prod * ck;
            sum += prod;

            /* update binomial for next m */
            if (m < cur) {
                binom = binom * (cur - m);
                binom = binom / (m + 1);      /* divide by small integer */
            }
        }
        a_seq[cur + 1] = sum;
    }

    seq_t result = a_seq[n];
    free(a_seq);
    free(pow2);
    return result;
}
