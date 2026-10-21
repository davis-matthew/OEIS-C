/*
 * A229394  Number of shapes of balanced 9-ary trees with n nodes, where a
 *          tree is balanced if the total number of nodes in subtrees
 *          corresponding to the branches of any node differ by at most one.
 * https://oeis.org/A229394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = C(9,r) * (a(m+1)^r) * (a(m)^(9-r)) where m =
 *            floor((n-1)/9) and r = (n-1) mod 9
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 19, u64 n <= 39, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t pow_big(seq_t base, unsigned exp) {
    seq_t result = 1;
    for (unsigned i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n) {
    /* C(9, r) values */
    static const seq_t comb9[10] = {
        1ULL, 9ULL, 36ULL, 84ULL, 126ULL,
        126ULL, 84ULL, 36ULL, 9ULL, 1ULL
    };

    /* static cache for computed values */
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;
    static size_t dp_last = 0;

    /* initialize cache */
    if (dp == NULL) {
        dp_cap = 1;
        dp = (seq_t *)malloc(dp_cap * sizeof(seq_t));
        dp[0] = 1;
        dp_last = 0;
    }

    /* enlarge cache if needed */
    if ((size_t)n > dp_cap - 1) {
        size_t new_cap = n + 1;
        if (new_cap < dp_cap * 2)
            new_cap = dp_cap * 2;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0;  /* allocation failure, return zero (unlikely) */
        dp = tmp;
        dp_cap = new_cap;
    }

    /* compute values up to n */
    for (idx_t i = dp_last + 1; i <= n; ++i) {
        idx_t s = i - 1;           /* remaining nodes after root */
        idx_t m = s / 9;           /* floor((n-1)/9)   */
        idx_t r = s % 9;           /* (n-1) mod 9     */

        seq_t A = dp[m];
        seq_t res;

        if (r == 0) {
            /* no larger subtrees; only size m */
            res = pow_big(A, 9);
        } else {
            seq_t B = dp[m + 1];
            res = pow_big(B, (unsigned)r) * pow_big(A, (unsigned)(9 - r));
        }
        res *= comb9[(int)r];
        dp[i] = res;
    }
    dp_last = (size_t)n;
    return dp[n];
}
