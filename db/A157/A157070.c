/*
 * A157070  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 42.
 * https://oeis.org/A157070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n}
 *            binomial(n+1,i)*binomial(20,i-1)*binomial(n-i+21,21)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 19, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not used for the OEIS offset */

    /* Pre‑compute the binomial factor binomial(n-i+21,21)
       for all i = 1..n  ->  for m = 21 .. n+20   */
    size_t sz = (size_t)n;
    seq_t *binom_m = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!binom_m) return 0;              /* allocation failure (unlikely) */

    seq_t cur = 1;                       /* binom(21,21) */
    binom_m[0] = cur;
    for (idx_t m = 22; m <= n + 20; ++m) {
        cur = cur * m / (m - 21);         /* recurrence */
        binom_m[m - 21] = cur;
    }

    seq_t sum = 0;
    seq_t b1   = n + 1;                   /* binom(n+1,1) */
    seq_t b2   = 1;                       /* binom(20,0) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t b3 = binom_m[n - i];        /* binom(n-i+21,21) */
        sum += b1 * b2 * b3;

        if (i < n) {
            /* Update b1: binom(n+1,i+1) = b1 * (n+1-i)/(i+1) */
            b1 = b1 * (n + 1 - i) / (i + 1);
            /* Update b2: binom(20,i)   = b2 * (20-(i-1))/i */
            b2 = b2 * (20 - (i - 1)) / i;
        }
    }

    free(binom_m);
    return sum;
}
