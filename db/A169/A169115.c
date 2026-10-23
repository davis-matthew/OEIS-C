/*
 * A169115  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 6·5^(n‑1) for 1 ≤ n ≤ 25, a(n) = 4·∑_{k=1}^{25}
 *            a(n‑k) – 10·a(n‑26) for n ≥ 26
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;   /* memoised sequence values */
    static size_t cap   = 0;     /* allocated capacity */
    static idx_t  len   = 0;     /* highest index computed so far (+1) */

    /* grow memo if necessary */
    if (n >= len) {
        size_t new_cap = cap ? cap : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *new_mem = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (!new_mem) abort();              /* out of memory */

        if (memo) {
            memcpy(new_mem, memo, len * sizeof(seq_t));
            free(memo);
        }
        memo = new_mem;
        cap   = new_cap;
    }

    /* compute values up to index n */
    for (idx_t i = len; i <= n; ++i) {
        if (i < 26) {
            if (i == 0) {
                memo[i] = 1;               /* a(0) */
            } else if (i == 1) {
                memo[i] = 6;               /* a(1) */
            } else {
                memo[i] = memo[i-1] * 5;   /* a(i) = a(i‑1)·5  for i≥2 */
            }
        } else {
            seq_t sum25 = 0;
            for (idx_t k = 1; k <= 25; ++k) {
                sum25 += memo[i - k];
            }
            memo[i] = 4 * sum25 - 10 * memo[i - 26];
        }
    }
    len = n + 1;          /* now memo[0..n] are valid */
    return memo[n];
}
