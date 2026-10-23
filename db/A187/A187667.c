/*
 * A187667  Coefficient of x^n in (1 + n*x + n*x^2 + n*x^3)^n.
 * https://oeis.org/A187667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in (1 + n*x + n*x^2 + n*x^3)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* allocate arrays for trinomial DP, size n+1 */
    size_t sz = (size_t)n + 1;
    seq_t *prev = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!prev || !cur) return 0;              /* out of memory – unlikely */

    /* row k = 0 : (1 + x + x^2)^0 */
    prev[0] = 1;
    for (size_t i = 1; i < sz; ++i) prev[i] = 0;

    seq_t ans = 0;
    seq_t binom = 1;   /* C(n,0) */
    seq_t pow_n = 1;   /* n^0 */

    /* iterate over k = 0 .. n */
    for (idx_t k = 0; k <= n; ++k) {
        size_t idx = (size_t)(n - k);          /* column n-k */
        seq_t v = prev[idx];                   /* T(k,n-k) */

        seq_t term = binom * pow_n;
        term = term * v;
        ans += term;

        if (k == n) break;   /* finished */

        /* compute next row: T(k+1, r) = T(k,r) + T(k,r-1) + T(k,r-2) */
        for (size_t r = 0; r < sz; ++r) {
            seq_t a = prev[r];
            seq_t b = (r >= 1) ? prev[r-1] : 0;
            seq_t c = (r >= 2) ? prev[r-2] : 0;
            cur[r] = a + b + c;
        }

        /* swap the rows for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;

        /* update binomial coefficient C(n,k+1) and n^(k+1) */
        seq_t mult = binom * (seq_t)(n - k);
        binom = mult / (seq_t)(k + 1);   /* exact division */
        pow_n = pow_n * (seq_t)n;
    }

    free(prev);
    free(cur);
    return ans;
}
