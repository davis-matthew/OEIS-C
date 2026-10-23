/*
 * A157063  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 28.
 * https://oeis.org/A157063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..n} binomial(n+1,i) * binomial(13,i-1) *
 *            binomial(n-i+14,14) (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 27, u128 n <= 142,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    if (n < 1)                   /* offset starts at 1 */
        return 0;

    /* Pre‑compute binomial(13,k), 0 ≤ k ≤ 13, once */
    static seq_t bin13[14];
    static int init = 0;
    if (!init) {
        for (int k = 0; k < 14; ++k)
            bin13[k] = binomial(13, k);
        init = 1;
    }

    seq_t sum = 0;

    /* binomial(n+1,i) with i = 1 initially */
    seq_t C_n1_i = (n + 1);          /* binomial(n+1,1) */

    /* binomial(n-i+14,14) with i = 1  →  binomial(n+13,14) */
    idx_t a_val   = n + 13;          /* current a = n-i+14 */
    seq_t C_a_14  = binomial(a_val, 14);

    /* we only need i up to 14 because binomial(13,i-1)=0 for i>14 */
    idx_t limit_i = n < 14 ? n : 14;

    for (idx_t i = 1; i <= limit_i; ++i) {
        seq_t term = C_n1_i * bin13[i - 1] * C_a_14;
        sum += term;

        if (i == limit_i) break;

        /* Update binomial(n+1,i) → binomial(n+1,i+1) */
        C_n1_i = (C_n1_i * (seq_t)(n + 1 - i)) / (seq_t)(i + 1);

        /* Update binomial(a_val,14) → binomial(a_val-1,14)  */
        seq_t a_old = (seq_t)a_val;
        seq_t num   = a_old - 14;   /* numerator (a-14) */
        seq_t den   = a_old;        /* denominator a */
        C_a_14 = (C_a_14 * num) / den;
        a_val--;                    /* next a = a_old-1 */
    }

    return sum;
}
