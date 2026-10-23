/*
 * A170252  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence satisfies a linear recurrence of order 40 derived
 *            from the generating function. For 0 ≤ n ≤ 40: a(n) = 1 + 2·(sum
 *            of a(n–k) for odd k) – (sum of a(n–k) for even k) For n > 40 :
 *            a(n) = 2·(sum of a(n–k) for odd k) – (sum of a(n–k) for even k)
 *            here the sums run over 1 ≤ k ≤ 40 and the t...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;           /* cache of computed values   */
    static idx_t memo_size = 0;          /* number of cached entries   */

    if (n >= memo_size) {
        /* grow cache to hold entries up to n */
        size_t new_size = n + 1;
        seq_t *tmp_mem = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!tmp_mem) { /* allocation failed – caller will likely abort */
            return 0;
        }
        memo = tmp_mem;

        /* compute missing terms */
        for (idx_t i = memo_size; i <= n; ++i) {
            if (i <= 40) {
                /* use the formula for the first 41 terms (including a(40)) */
                seq_t odd_sum  = 0;
                seq_t even_sum = 0;
                for (idx_t k = 1; k <= i; ++k) {
                    if ((k & 1) == 0)           /* even k -> +1 coefficient */
                        even_sum += memo[i - k];
                    else                         /* odd k  -> –2 coefficient */
                        odd_sum += memo[i - k];
                }
                seq_t tmp = odd_sum + odd_sum;          /* 2 * odd_sum   */
                memo[i] = tmp + 1 - even_sum;           /* 1 + 2*odd_sum – even_sum */
            } else {
                /* recurrence for n > 40:  a(n) = 2*odd_sum – even_sum  */
                seq_t odd_sum  = 0;
                seq_t even_sum = 0;
                for (idx_t j = 1; j <= 40; ++j) {
                    seq_t val = memo[i - j];
                    if (j & 1)                       /* odd j  -> +2 coefficient */
                        odd_sum += val;
                    else                             /* even j -> –1 coefficient */
                        even_sum += val;
                }
                seq_t tmp = odd_sum + odd_sum;          /* 2 * odd_sum   */
                memo[i] = tmp - even_sum;                /* 2*odd_sum – even_sum   */
            }
        }
        memo_size = new_size;
    }
    return memo[n];
}
