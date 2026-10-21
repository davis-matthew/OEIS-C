/*
 * A228997  Total sum of the 7th powers of lengths of ascending runs in all
 *          permutations of [n].
 * https://oeis.org/A228997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228997
   Total sum of the 7th powers of lengths of ascending runs in all permutations of [n].
   Recurrence derived from the given EGF:
     a(0)=0, a(1)=1.
     For n≥2
        a(n)=2*n*a(n-1)-n*(n-1)*a(n-2)
              + p0*(n)_0 + p1*(n)_1 + p2*(n)_2 + p3*(n)_3 + p4*(n)_4 + p5*(n)_5
   where
        p = { -126, 126, 0, 280, 210, 42 }
        (n)_k = n·(n-1)·…·(n-k+1)   (rising factorial, (n)_0=1).
   The array is memoised for arbitrary‐order calls.
*/

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static memoisation */
    static seq_t *memo = NULL;
    static size_t memo_cap = 0;
    static size_t memo_len = 0;   /* highest index computed (inclusive) */

    /* ensure allocation capacity */
    if (n >= memo_cap) {
        size_t new_cap = (memo_cap > 0) ? memo_cap * 2 : 8;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed */
        memo = tmp;
        memo_cap = new_cap;
    }

    /* compute terms up to n if not yet computed */
    if (memo_len < n) {
        if (memo_len == 0) {        /* initialise first two terms */
            memo[0] = 0;
            memo[1] = 1;
            memo_len = 1;
        }

        static const int p[6] = { -126, 126, 0, 280, 210, 42 };

        for (idx_t i = memo_len + 1; i <= n; ++i) {
            if (i == 0) {
                memo[0] = 0;
                continue;
            }
            if (i == 1) {
                memo[1] = 1;
                continue;
            }

            /* compute RHS sum of p_k * (i)_k */
            seq_t rhs = 0;
            for (int k = 0; k <= 5; ++k) {
                if (i < (idx_t)k) continue;          /* term vanishes */
                seq_t term = 1;
                for (int j = 0; j < k; ++j) {
                    term *= (seq_t)(i - (idx_t)j);
                }
                rhs += (seq_t)p[k] * term;
            }

            /* recurrence: a(i) = rhs + 2*i*a(i-1) - i*(i-1)*a(i-2) */
            seq_t val = rhs;
            val += (seq_t)2 * (seq_t)i * memo[i - 1];
            val -= (seq_t)i * (seq_t)(i - 1) * memo[i - 2];
            memo[i] = val;
        }
        memo_len = n;
    }

    return memo[n];
}
