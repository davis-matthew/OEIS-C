/*
 * A110166  Row sums of Riordan array A110165.
 * https://oeis.org/A110166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(5^n + Sum_{k=0..n} C(n,k)*C(2*k,k)) Recurrence:
 *            a(0)=1, a(1)=4, a(2)=18 n*a(n) = (11*n-8)*a(n-1) -
 *            5*(7*n-10)*a(n-2) + 25*(n-2)*a(n-3) (n≥3) The implementation
 *            uses the recurrence with memoised values
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 171 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

seq_t a_wide(idx_t n) {
    static seq_t *dp = NULL;
    static idx_t dp_cap = 0;   /* current allocation size */
    static idx_t dp_len = 0;   /* number of computed elements */

    /* allocate or grow the cache */
    if (dp == NULL) {
        dp_cap = 4;
        dp = mem_new(seq_t, dp_cap);
        if (!dp) return (seq_t)0;   /* allocation failure – return 0 */
    } else if (n >= dp_cap) {
        idx_t new_cap = dp_cap;
        while (n >= new_cap) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, dp, new_cap);
        if (!tmp) return (seq_t)0;   /* allocation failure */
        dp = tmp;
        dp_cap = new_cap;
    }

    /* initialise base values if not yet done */
    if (dp_len == 0) {
        dp[0] = (seq_t)1;
        if (dp_cap > 1) dp[1] = (seq_t)4;
        if (dp_cap > 2) dp[2] = (seq_t)18;
        dp_len = 3;
    }

    /* compute up to n if necessary */
    for (idx_t i = dp_len; i <= n; ++i) {
        if (i == 0) { dp[i] = (seq_t)1; continue; }
        if (i == 1) { dp[i] = (seq_t)4; continue; }
        if (i == 2) { dp[i] = (seq_t)18; continue; }

        seq_t term1 = ((seq_t)(11 * i - 8)) * dp[i - 1];
        seq_t term2 = ((seq_t)(5 * (7 * i - 10))) * dp[i - 2];
        seq_t term3 = ((seq_t)(25 * (i - 2))) * dp[i - 3];
        dp[i] = (term1 - term2 + term3) / (seq_t)i;
    }

    dp_len = n + 1;
    return dp[n];
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
