/*
 * A170248  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (45 * sum_{k=1}^{min(39,n)} a(n-k)) + c(n) where c(n) = 2
 *            for 1 ≤ n ≤ 39, 1 for n = 40, 0 for n ≥ 41. This recurrence
 *            follows from the rational generating function given in the OEIS
 *            entry
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts reduced words in the Coxeter group on 47 generators
   with all (S_i,S_j) relations of order 40.  The sequence starts
   1,47,2162,99452,4574792,…  */
#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_size = 0;

static void ensure_size(idx_t n)
{
    if (n + 1 <= memo_size) return;
    /* Grow in chunks of at least double the current size or to n+1. */
    idx_t new_size = (memo_size > 0) ? memo_size * 2 : 1;
    while (new_size <= n) new_size = new_size * 2;
    /* Allocate or reallocate.  The returned pointer may be NULL; in
       that case the program will abort, which is acceptable for this
       library build. */
    seq_t *tmp = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
    memo = tmp;
    memo_size = new_size;
}

seq_t a(idx_t n)
{
    /* Memoize the sequence up to n on demand. */
    if (n >= memo_size) {
        ensure_size(n);
    }

    /* Compute terms incrementally.  We keep track of the last
       computed index. */
    static idx_t last_computed = 0;  /* index of last computed term */

    /* Initialize a(0) if not already. */
    if (last_computed == 0 && memo[0] == 0) {
        memo[0] = (seq_t)1;            /* a(0) = 1 */
        last_computed = 0;
    }

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        int kmax = (i < 40) ? (int)i : 39;
        for (int k = 1; k <= kmax; ++k) {
            sum += memo[i - k];
        }

        if (i < 40) {
            memo[i] = sum * (seq_t)45 + (seq_t)2;
        } else if (i == 40) {
            memo[i] = sum * (seq_t)45 - memo[0] * (seq_t)1035 + (seq_t)1;
        } else {
            memo[i] = sum * (seq_t)45 - memo[i - 40] * (seq_t)1035;
        }
    }
    last_computed = n;
    return memo[n];
}
