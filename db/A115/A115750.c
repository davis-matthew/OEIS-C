/*
 * A115750  a(n) = Sum_{k=0..2*n} binomial(2*n, k)*binomial(3*n, 2*n + 2*k).
 * https://oeis.org/A115750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(2*n, k)*binomial(3*n, 2*n +
 *            2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation uses the library routine binomial(n,k).  It iteratively
 * builds the binomial coefficients B1(k)=C(2n,k) and B2(k)=C(3n,2n+2k)
 * so that only elementary arithmetic on seq_t is required.  The loop
 * runs from k=0 to ⌊n/2⌋ because for larger k the second binomial is
 * zero.  All arithmetic is performed in seq_t, with the idx_t values
 * cast to seq_t only after the required integer arithmetic has been
 * completed to avoid undefined behaviour.  The sequence starts at
 * a(0)=1, matching the OEIS offset. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* B1(k) = binomial(2*n, k), B2(k) = binomial(3*n, 2*n + 2*k)   */
    seq_t b1 = 1;                           /* at k = 0, C(2n,0) = 1       */
    seq_t b2 = binomial((idx_t)(3*n), (idx_t)(2*n));  /* at k = 0                */

    /* pre‑compute some small integers in idx_t to avoid overflow */
    idx_t two_n   = n + n;                  /* 2*n                       */
    idx_t three_n = two_n + n;              /* 3*n                       */
    idx_t max_k   = n / 2;                  /* terms with non‑zero B2     */

    for (idx_t k = 0; k <= max_k; ++k) {
        /* accumulate the current term   */
        sum += b1 * b2;

        /* update for next k (unless last iteration) */
        if (k == max_k) break;

        /* --- update B1(k) -> B1(k+1) --- */
        /*  B1(k+1) = B1(k) * (2*n - k) / (k+1)   */
        idx_t two_n_minus_k = two_n - k;     /* guaranteed non‑negative   */
        b1 = b1 * (seq_t)two_n_minus_k
               / (seq_t)(k + 1);

        /* --- update B2(k) -> B2(k+1) --- */
        /*  B2(k+1) = B2(k) *
         *               (3*n - 2*n - 2*k) *
         *               (3*n - 2*n - 2*k - 1)
         *           / ((2*n + 2*k + 1)*(2*n + 2*k + 2))   */
        idx_t two_k = k + k;                 /* 2*k                       */
        idx_t r     = two_n + two_k;         /* 2*n + 2*k                 */

        seq_t num1 = (seq_t)(three_n - r);          /* n - 2*k          */
        seq_t num2 = (seq_t)(three_n - r - 1);      /* n - 2*k - 1      */
        seq_t den1 = (seq_t)(r + 1);                /* 2*n + 2*k + 1    */
        seq_t den2 = (seq_t)(r + 2);                /* 2*n + 2*k + 2    */

        b2 = b2 * num1;
        b2 = b2 / den1;
        b2 = b2 * num2;
        b2 = b2 / den2;
    }

    return sum;
}
