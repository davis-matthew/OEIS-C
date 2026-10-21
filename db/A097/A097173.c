/*
 * A097173  Total number of green nodes among tricolored labeled trees on n
 *          nodes.
 * https://oeis.org/A097173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Sum_{l=1..n} [ (-1)^l * (l-1) * l^{l-1} * C(n,l) *
 *            n^{n-1-l} ]
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;                 /* a(1) = 0 */

    /* n^(n-1) */
    seq_t n_pow_n1 = pow_seq((seq_t)n, n - 1);

    seq_t sum_pos = 0, sum_neg = 0;        /* will hold positive and negative contributions */

    seq_t comb = 1;                        /* C(n,0) = 1 */
    seq_t n_power_l = 1;                   /* will hold n^l, starting with n^0 = 1 */

    for (idx_t l = 1; l <= n; ++l)
    {
        /* n^l */
        n_power_l *= (seq_t)n;

        /* binomial coefficient C(n,l) */
        comb = (comb * (seq_t)(n - l + 1)) / (seq_t)l;    /* integer division is exact */

        /* l^{l-1} = l^l / l */
        seq_t l_pow   = pow_seq((seq_t)l, l);
        seq_t temp    = l_pow / (seq_t)l;                 /* l^{l-1} */

        temp *= (seq_t)(l - 1);                          /* (l-1) * l^{l-1} */
        temp *= comb;                                    /* * C(n,l) */

        seq_t term;

        if (l == n)
        {
            /* special case: l = n, n^{n-1-l} = n^{-1} is canceled by the (l-1) factor.
               Term reduces to (n-1) * n^{n-2}. */
            seq_t n_pow_n2 = n_pow_n1 / (seq_t)n;         /* n^{n-2} */
            term = (seq_t)(n - 1) * n_pow_n2;
        }
        else
        {
            /* n^{n-1-l} = n_pow_n1 / n^l */
            seq_t pow_n_part = n_pow_n1 / n_power_l;
            term = temp * pow_n_part;
        }

        /* accumulate with correct sign: (-1)^l */
        if (l & 1)
            sum_neg += term;   /* odd l -> negative term */
        else
            sum_pos += term;   /* even l -> positive term */
    }

    seq_t result = sum_pos;
    result -= sum_neg;
    result *= 2;                  /* final factor of 2 */

    return result;
}
