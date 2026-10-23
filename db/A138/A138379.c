/*
 * A138379  Number of embedded coalitions in an n-person game where the
 *          position of the individual player is important.
 * https://oeis.org/A138379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(2) = 6 a(n) = n! * [ Σ_{i=2}^{n-1} C(n,i) * (
 *            Σ_{j=1}^{i-1} a(j)/j! ) + Σ_{i=0}^{n-1} C(n,i) ], n > 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;

    /* allocate arrays for b(k) = a(k)/k! and their prefix sums S(k) = Σ_{j=1}^{k} b(j) */
    size_t size = (size_t)(n + 1);
    seq_t *b = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *S = (seq_t *)malloc(size * sizeof(seq_t));
    if (!b || !S) { /* memory failure – return zero */
        return (seq_t)0;
    }

    /* initialise for n = 1 and n = 2 */
    b[1] = (seq_t)1;
    S[1] = b[1];

    b[2] = (seq_t)3;            /* a(2)/2! = 6/2 = 3 */
    S[2] = S[1] + b[2];

    /* factorial and power of two for the current n */
    seq_t fact = (seq_t)2;      /* 2!   (value after handling n = 2) */
    seq_t pow2 = (seq_t)4;      /* 2²   (value after handling n = 2) */

    /* compute for n = 3 … n */
    for (idx_t cur = 3; cur <= n; ++cur) {
        fact *= (seq_t)cur;        /* cur!  */
        pow2 *= (seq_t)2;          /* 2^cur */

        seq_t term1 = pow2 - (seq_t)1;  /* Σ_{i=0}^{cur-1} C(cur,i) = 2^cur - 1 */
        seq_t sum   = term1;

        seq_t binom = (seq_t)1;  /* C(cur,0) */
        for (idx_t i = 1; i < cur; ++i) {
            /* update binomial coefficient C(cur,i) */
            binom = binom * (seq_t)(cur - i + 1) / (seq_t)i;
            if (i >= 2) {
                sum += binom * S[i - 1];
            }
        }

        b[cur] = sum;
        S[cur] = S[cur - 1] + sum;
    }

    /* a(n) = n! * b(n) */
    seq_t result = fact * b[n];

    free(b);
    free(S);
    return result;
}
