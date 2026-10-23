/*
 * A164630  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1
 *         a(1) = 18
 *         a(2) = 306
 *         a(3) = 5202
 *         a(4) = 88434
 *         a(5) = 1503378
 *         a(6) = 25557426
 * For n ≥ 7:  a(n) = 16*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6))
 *               – 136*a(n-7) + [n==7]   (the +1 term comes from the numerator of the G.f.)
 *
 * This implements the rational generating function
 *   (t^7 + 2t^6 + 2t^5 + 2t^4 + 2t^3 + 2t^2 + 2t + 1) /
 *   (136t^7 – 16t^6 – 16t^5 – 16t^4 – 16t^3 – 16t^2 – 16t + 1).
 *
 * The function memoises computed values so it works in any call order.
 */

#include <stdlib.h>
#include <stdint.h>

/* memoisation buffer */
static seq_t *memo = NULL;
static size_t memo_cap = 0;

seq_t a(idx_t n)
{
    /* small known initial values */
    static const seq_t init[7] = {
        (seq_t)1,
        (seq_t)18,
        (seq_t)306,
        (seq_t)5202,
        (seq_t)88434,
        (seq_t)1503378,
        (seq_t)25557426
    };

    /* size_t can hold up to at least 8 bytes; guard against huge n */
    if (n >= SIZE_MAX) {
        /* not enough memory to store the requested index */
        return (seq_t)0;
    }

    /* resize memoisation array if necessary */
    if ((size_t)n + 1 > memo_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – return zero */
            return (seq_t)0;
        }
        memo = tmp;

        /* compute values up to index n */
        for (idx_t i = memo_cap; i <= n; ++i) {
            idx_t ui = (idx_t)i;           /* unsigned loop index */
            seq_t val;

            if (ui < 7) {
                val = init[ui];
            } else if (ui == 7) {
                /* special case for n == 7: include the "+1" from numerator */
                seq_t sum = memo[6] + memo[5] + memo[4] + memo[3] + memo[2] + memo[1];
                val = (seq_t)16 * sum - (seq_t)136 * memo[0] + (seq_t)1;
            } else {
                /* standard recurrence for n >= 8 */
                seq_t sum = memo[i-1] + memo[i-2] + memo[i-3] +
                            memo[i-4] + memo[i-5] + memo[i-6];
                val = (seq_t)16 * sum - (seq_t)136 * memo[i-7];
            }
            memo[i] = val;
        }
        memo_cap = new_cap;
    }

    return memo[n];
}
