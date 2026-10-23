/*
 * A170625  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 38 * (a(n-1) + a(n-2) + … + a(n-47)) – 741 * a(n-48) +
 *            r(n) r(0) = 1 r(1)…r(47) = 2 r(48) = 1 r(n) = 0 for n > 48
 *            Initial values: a(0)=1, a(1)=40, a(2)=1560. This recurrence
 *            comes from the rational generating function of A170625
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *dp   = NULL;
    static size_t dp_len = 0;   /* number of terms currently stored */
    static size_t dp_cap = 0;   /* allocated capacity */

    if ((size_t)n < dp_len)        /* value already cached */
        return dp[(size_t)n];

    /* ensure the array is large enough (exponential growth) */
    size_t need = (size_t)n + 1;
    if (need > dp_cap) {
        size_t new_cap = dp_cap ? dp_cap : 1;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);       /* allocation failed – terminate */
        dp = tmp;
        dp_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t idx = dp_len; idx <= (size_t)n; ++idx) {
        seq_t rhs = 0;
        if (idx == 0)      rhs = 1;
        else if (idx <= 47)rhs = 2;
        else if (idx == 48)rhs = 1;

        seq_t val = 0;

        size_t up = (idx < 47) ? idx : 47;
        for (size_t i = 1; i <= up; ++i)
            val += 38 * dp[idx - i];

        if (idx >= 48)
            val -= 741 * dp[idx - 48];

        val += rhs;
        dp[idx] = val;
    }

    dp_len = need;
    return dp[(size_t)n];
}
