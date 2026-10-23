/*
 * A157074  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 50.
 * https://oeis.org/A157074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1}^{min(n,25)} C(n+1,i) * C(24,i-1) * C(n-i+25,25)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 18, u128 n <= 56,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Precompute C(24,j) for j=0..24 */
    static int init = 0;
    static seq_t c24[25];
    if (!init) {
        for (idx_t j = 0; j <= 24; ++j)
            c24[j] = binomial(24, j);
        init = 1;
    }

    idx_t max_i = (n < 25) ? n : 25;      /* because C(24,i-1)=0 for i>25 */
    seq_t sum   = 0;
    seq_t c_n1i = 1;                       /* C(n+1,0) */

    for (idx_t i = 1; i <= max_i; ++i) {
        /* Update C(n+1,i) from C(n+1,i-1) */
        c_n1i = (c_n1i * (seq_t)(n + 2 - i)) / (seq_t)i;   /* exact division */

        seq_t term = c_n1i * c24[i - 1];
        term *= binomial(n - i + 25, 25);
        sum += term;
    }

    return sum;
}
