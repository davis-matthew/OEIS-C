/*
 * A102247  Number of partitions of n in which each odd part has odd
 *          multiplicity and each even part has even multiplicity.
 * https://oeis.org/A102247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{i>0}
 *            (1+x^(2*i-1)-x^(4*i-2))/(1-x^(2*i))
 * Range:     u8 n <= 27, u16 n <= 70, u32 n <= 204, u64 n <= 658,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0) {
        return (seq_t)1;
    }

    /* work with a normal int index for small n (fast enough for 10000) */
    int N = (int)n;

    /* allocate the three working arrays */
    seq_t *dp      = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *dpF     = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *nextdp  = (seq_t *)calloc(N + 1, sizeof(seq_t));

    if (!dp || !dpF || !nextdp) { /* allocation failure */
        exit(1);
    }

    dp[0] = (seq_t)1;  /* base case */

    /* iterate over i = 1 .. n (the factor index i) */
    for (int i = 1; i <= N; ++i) {
        int c = 2 * i;          /* denominator exponent 2i */
        int a = c - 1;          /* odd part exponent 2i-1 */

        /* compute dpF = dp * (1 + x^c + x^(2c) + ...) up to degree N */
        for (int r = 0; r < c && r <= N; ++r) {
            seq_t s = 0;
            for (int idx = r; idx <= N; idx += c) {
                s += dp[idx];
                dpF[idx] = s;
            }
        }

        /* form nextdp = dpF + shift(dpF, a) - shift(dpF, 2a)  (coefficients +1,+1,-1) */
        for (int idx = 0; idx <= N; ++idx) {
            seq_t val = dpF[idx];
            if (idx >= a) {
                val += dpF[idx - a];
            }
            if (idx >= 2 * a) {
                val -= dpF[idx - 2 * a];
            }
            nextdp[idx] = val;
        }

        /* swap dp and nextdp for the next iteration */
        seq_t *tmp = dp;
        dp = nextdp;
        nextdp = tmp;
    }

    seq_t result = dp[N];

    free(dp);
    free(dpF);
    free(nextdp);

    return result;
}
