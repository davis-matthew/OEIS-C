/*
 * A151627  Number of permutations of 2 indistinguishable copies of 1..n with
 *          exactly 5 adjacent element pairs in decreasing order.
 * https://oeis.org/A151627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..5} (-1)^j * binomial(2*n+1, j) * binomial(7-j,
 *            2)^n
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Returns the nth term of A151627 for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* behaviour below the offset is irrelevant */

    /* binomial(7-j, 2) for j = 0..5 */
    static const seq_t base_vals[6] = {21, 15, 10, 6, 3, 1};

    seq_t sum_pos = 0;   /* terms with even j (sign +) */
    seq_t sum_neg = 0;   /* terms with odd j  (sign -) */

    idx_t two_n_plus1 = 2 * n + 1;

    for (idx_t j = 0; j <= 5; ++j)
    {
        seq_t bj   = binomial(two_n_plus1, j);                 /* C(2n+1, j) */
        seq_t powj = seq_pow(base_vals[j], n);                 /* (C(7-j,2))^n */
        seq_t term = seq_mul(bj, powj);                       /* term = bj * powj */

        if ((j & 1) == 0)
            sum_pos += term;    /* even j → + */
        else
            sum_neg += term;    /* odd  j → - */
    }

    return sum_pos - sum_neg;   /* guaranteed non‑negative */
}
