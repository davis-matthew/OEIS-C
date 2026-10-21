/*
 * A218216  G.f.: Sum_{n>=0} x^n * Sum_{k=0..n} binomial(n,k)^4 *
 *          x^k*(1-x)^(n-k).
 * https://oeis.org/A218216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in A(x) = Σ_{n≥0} x^n Σ_{k=0}^n
 *            binom(n,k)^4 x^k (1−x)^{n−k}
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return the n-th term of sequence A218216 */
seq_t a(idx_t n)
{
    seq_t pos = 0;          /* sum of terms with positive sign   */
    seq_t neg = 0;          /* sum of terms with negative sign  */

    /* i ranges from ceil(n/2) to n */
    idx_t i_start = (n + 1) / 2;
    for (idx_t i = i_start; i <= n; ++i)
    {
        idx_t r = n - i;   /* exponent difference            */

        /* ----- compute initial binom(i,0) and B_0 = C(i,r) ----- */
        seq_t binom = 1;    /* C(i,0) */
        seq_t B_cur = 1;    /* will become C(i,r) */

        for (idx_t t = 1; t <= r; ++t)
        {
            B_cur = B_cur * (seq_t)(i - t + 1) / (seq_t)t;
        }

        bool sign_neg = (r & 1) != 0; /* (-1)^r */

        /* ----- inner loop over k = 0..r ----- */
        for (idx_t k = 0; k <= r; ++k)
        {
            /* term = binom^4 * B_cur */
            seq_t pow4 = binom * binom;
            pow4 = pow4 * binom * binom;
            seq_t term = pow4 * B_cur;

            if (sign_neg)
                neg += term;
            else
                pos += term;

            /* prepare for next k (if any) */
            if (k < r)
            {
                /* toggle sign */
                sign_neg = !sign_neg;

                /* update binom to C(i,k+1) */
                binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);

                /* update B_cur to C(i-(k+1), r-(k+1)) */
                B_cur = B_cur * (seq_t)(r - k) / (seq_t)(i - k);
            }
        }
    }

    /* result is positive difference of the two sums */
    return pos - neg;
}
