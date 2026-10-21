/*
 * A217145  exp( Sum_{n>=1} x^n/n^4 ) = Sum_{n>=0} a(n)*x^n/n!^4.
 * https://oeis.org/A217145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (n-1)!·(n!)³·Σ_{k=0}^{n-1} a(k)/( (k!)⁴·(n‑k)³
 *            ) (OEIS A217145)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cached factorials and sequence values */
    static seq_t *fact = NULL;          /* fact[i] = i!    */
    static seq_t *seq  = NULL;          /* seq[i]  = a(i)  */
    static size_t fact_cap = 0;
    static size_t seq_cap  = 0;

    /* ensure factorial array contains 0! … n! */
    if (fact_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) return 0;   /* allocation failed */
        fact = tmp;
        for (size_t i = fact_cap; i <= n; ++i) {
            if (i == 0)
                fact[i] = (seq_t)1;
            else
                fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_cap = new_cap;
    }

    /* ensure sequence array contains 0 … n */
    if (seq_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) return 0;   /* allocation failed */
        seq = tmp;
        for (size_t i = seq_cap; i <= n; ++i) {
            if (i == 0) {
                seq[i] = (seq_t)1;
            } else {
                /* build constants for this n */
                seq_t fact_nm1 = fact[i - 1];
                seq_t fact_n  = fact[i];

                /* n!³ */
                seq_t fact_n3 = fact_n;
                fact_n3 *= fact_n;
                fact_n3 *= fact_n;

                /* const_part = (n-1)!·(n!)³ */
                seq_t const_part = fact_nm1 * fact_n3;

                seq_t sum = (seq_t)0;
                for (idx_t k = 0; k <= (idx_t)(i - 1); ++k) {
                    seq_t fact_k = fact[(size_t)k];

                    /* (k!)⁴ */
                    seq_t fact_k4 = fact_k;
                    fact_k4 *= fact_k;
                    fact_k4 *= fact_k;
                    fact_k4 *= fact_k;

                    /* (n-k)³ */
                    seq_t ik  = (seq_t)(i - k);
                    seq_t ik3 = ik * ik * ik;

                    /* denominator = (k!)⁴ · (n-k)³ */
                    seq_t denom = fact_k4 * ik3;

                    /* reduce = const_part / denom (exact) */
                    seq_t reduce = const_part / denom;
                    seq_t term   = reduce * seq[(size_t)k];
                    sum += term;
                }
                seq[i] = sum;
            }
        }
        seq_cap = new_cap;
    }
    return seq[(size_t)n];
}
