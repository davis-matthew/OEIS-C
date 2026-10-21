/*
 * A061697  Generalized Bell numbers.
 * https://oeis.org/A061697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=0 For n ≥ 3: a(n) = Σ_{k=0}^{n-3} a(k) *
 *            C(n-1,k) * C(n,k) where C(x,y) is the binomial coefficient. This
 *            follows from the e.g.f. Σ a(n) x^n/(n!)^2 = exp( Σ_{m≥3}
 *            x^m/(m!)^2 )
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static memoization array */
    static seq_t *memo   = NULL;
    static idx_t memo_len = 0;   /* number of stored elements */

    if (n < memo_len)          /* already computed */
        return memo[n];

    /* enlarge the memo array if necessary */
    size_t new_len = (size_t)n + 1;
    if (memo_len == 0) {               /* first call */
        memo = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!memo) abort();             /* allocation failed */
        memo_len = 0;
    } else if (new_len > memo_len) {    /* expand */
        memo = (seq_t*)realloc(memo, new_len * sizeof(seq_t));
        if (!memo) abort();             /* allocation failed */
        memo_len = 0;                   /* will rebuild up to n */
    }

    /* set known initial values if they are not already set */
    if (new_len > 0 && memo_len < 1) memo[0] = 1;
    if (new_len > 1 && memo_len < 2) memo[1] = 0;
    if (new_len > 2 && memo_len < 3) memo[2] = 0;
    if (new_len > 2) memo_len = 3;

    /* compute values from memo_len up to n */
    for (idx_t cur = memo_len; cur <= n; ++cur) {
        if (cur <= 2) {                 /* already initialized */
            memo[cur] = (cur==0 ? 1 : 0);
            continue;
        }

        seq_t sum  = 0;
        seq_t bin1 = 1;   /* C(cur-1,0) */
        seq_t bin2 = 1;   /* C(cur,0)   */

        idx_t limit = cur - 3;          /* k goes to cur-3 */

        for (idx_t k = 0; k <= limit; ++k) {
            sum += memo[(size_t)k] * bin1 * bin2;
            if (k < limit) {
                /* Update bin1 = C(cur-1, k+1)  and  bin2 = C(cur, k+1) */
                bin1 = bin1 * (seq_t)(cur - 1 - k) / (seq_t)(k + 1);
                bin2 = bin2 * (seq_t)(cur - k)     / (seq_t)(k + 1);
            }
        }
        memo[(size_t)cur] = sum;
    }

    memo_len = new_len;          /* store the new length */
    return memo[(size_t)n];
}
