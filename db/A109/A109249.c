/*
 * A109249  Expansion of x*(-1+2*x-x^2+7*x^3+8*x^4-7*x^5+8*x^6) /
 *          ((4*x^3-x^2+3*x-1)*(2*x^4-2*x^3+3*x^2+1)*(x-1)^2).
 * https://oeis.org/A109249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 11*a(n-2) + 26*a(n-3) - 45*a(n-4) + 57*a(n-5)
 *            - 61*a(n-6) + 48*a(n-7) - 26*a(n-8) + 8*a(n-9) Initial values:
 *            a(0)=0,a(1)=1,a(2)=3,a(3)=5,a(4)=11,a(5)=25,
 *            a(6)=63,a(7)=203,a(8)=627
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small constants for the recurrence */
    const int  coeff_pos[5] = {5, 26, 57, 48, 8};   /* positive coefficients   */
    const idx_t idx_pos[5]  = {1, 3, 5, 7, 9};        /* corresponding offsets  */
    const int  coeff_neg[4] = {11, 45, 61, 26}; /* negative coefficients   */
    const idx_t idx_neg[4]  = {2, 4, 6, 8};        /* corresponding offsets  */

    /* Base values for n < 9 */
    static const seq_t base[9] = {
        0ULL, 1ULL, 3ULL, 5ULL, 11ULL,
        25ULL, 63ULL, 203ULL, 627ULL
    };

    if (n < 9) /* quick return of the known values */
        return base[n];

    /* Memoisation storage for previously computed terms */
    static seq_t *memo  = NULL;    /* array of terms */
    static size_t memo_cap = 0;    /* allocated capacity */
    static size_t memo_len = 0;    /* number of terms currently stored */

    /* Initialise the memoisation array on the first call */
    if (memo_len == 0) {
        memo_cap = 12;                 /* some spare room */
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return 0;           /* allocation failure – unlikely */
        for (size_t i = 0; i < 9; ++i)
            memo[i] = base[i];
        memo_len = 9;                  /* we have terms 0..8 ready */
    }

    /* Grow the array if the requested index is beyond the current capacity */
    if ((size_t)n >= memo_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure – unlikely */
        memo   = tmp;
        memo_cap = new_cap;
    }

    /* If we already have the term, return it immediately */
    if ((size_t)n < memo_len)
        return memo[n];

    /* Compute terms sequentially up to the requested index */
    for (size_t i = memo_len; i <= (size_t)n; ++i) {
        seq_t pos_sum = 0;   /* sum of positive‑coefficient terms   */
        seq_t neg_sum = 0;   /* sum of negative‑coefficient terms   */

        /* positive coefficients */
        pos_sum += (seq_t)coeff_pos[0] * memo[i - idx_pos[0]];
        pos_sum += (seq_t)coeff_pos[1] * memo[i - idx_pos[1]];
        pos_sum += (seq_t)coeff_pos[2] * memo[i - idx_pos[2]];
        pos_sum += (seq_t)coeff_pos[3] * memo[i - idx_pos[3]];
        pos_sum += (seq_t)coeff_pos[4] * memo[i - idx_pos[4]];

        /* negative coefficients */
        neg_sum += (seq_t)coeff_neg[0] * memo[i - idx_neg[0]];
        neg_sum += (seq_t)coeff_neg[1] * memo[i - idx_neg[1]];
        neg_sum += (seq_t)coeff_neg[2] * memo[i - idx_neg[2]];
        neg_sum += (seq_t)coeff_neg[3] * memo[i - idx_neg[3]];

        /* Since the final value is guaranteed to be positive,
           we can safely compute the difference. */
        memo[i] = pos_sum - neg_sum;
    }

    memo_len = n + 1;
    return memo[n];
}
