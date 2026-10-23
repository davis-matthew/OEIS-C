/*
 * A186364  Number of cycle-up-down permutations of {1,2,...,n} having no
 *          fixed points. A permutation is said to be cycle-up-down if it is a
 *          product of up-down cycles. A cycle (b(1), b(2), ...) is said to be
 *          up-down if, when written with its smallest element in the first
 *          position, it satisfies b(1)<b(2)>b(3)...
 * https://oeis.org/A186364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of cycle‑up‑down permutations of {1,…,n} having no
 *            fixed points. Recurrence derived from the exponential generating
 *            function F(z) = exp(-z)/(1-sin(z)): a(0) = 1, a(1) = 0, for n >=
 *            2: a(n) = (-1)^n + Sum_{odd m=1..n} (-1)^{(m-1)/2} * binom(n,m)
 *            * a(n-m)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses only integer arithmetic with unsigned types.  For arbitrary–precision
   builds, seq_t will be a signed big integer class, so negative intermediates that are
   temporarily generated are represented as a difference of two non‑negative terms.  This
   avoids the need for signed arithmetic on unsigned machines and guarantees correct
   results for any seq_t that can represent the final value. */

/* --------------------------------------------------------------- */

#include <stdlib.h>  /* for malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoised values  */
    static idx_t cache_size = 0;     /* number of cached entries  */

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        /* initialise new entries */
        for (idx_t i = old_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            if (i == 1) {
                cache[1] = (seq_t)0;
                continue;
            }

            /* compute a(i) by the recurrence */
            seq_t sum_pos = (seq_t)0;
            seq_t sum_neg = (seq_t)0;

            /* binomial coefficient C(i, m) for odd m,
               and sign for the term.  sign = +1 for
               m = 1, 5, 9, …  (i.e. (m-1)/2 even). */
            seq_t binom = (seq_t)i;   /* C(i,1)   */
            int sign = +1;            /* for m=1   */

            for (idx_t m = 1; m <= i; m += 2) {
                seq_t term = binom * cache[i - m];
                if (sign == +1)
                    sum_pos += term;
                else
                    sum_neg += term;

                /* compute C(i, m+2) from C(i, m) */
                idx_t next_m = m + 2;
                if (next_m > i)
                    break;

                seq_t mult1 = (seq_t)(i - m);
                seq_t mult2 = (seq_t)(i - m - 1);
                seq_t denom = (seq_t)(m + 1) * (seq_t)(m + 2);

                binom = (binom * mult1 * mult2) / denom;
                sign = -sign;
            }

            seq_t val = sum_pos - sum_neg;
            if ((i & 1) == 0)   /* n even: add 1 */
                val += (seq_t)1;
            else                /* n odd : subtract 1 */
                val -= (seq_t)1;

            cache[i] = val;
        }
    }
    return cache[n];
}
