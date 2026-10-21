/*
 * A079262  Octanacci numbers: a(0)=a(1)=...=a(6)=0, a(7)=1; for n >= 8, a(n)
 *          = Sum_{i=1..8} a(n-i).
 * https://oeis.org/A079262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8) *
 *            with initial values a(0)=…=a(6)=0, a(7)=1. This function
 *            memoises computed * terms in a dynamic array so that subsequent
 *            calls (in any order) are fast
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 40, u64 n <= 72, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Persistent storage of computed terms.  The array grows as needed. */
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;   /* number of terms actually stored */
    static idx_t memo_cap = 0;   /* allocated capacity */

    /* Initialise the first eight terms on the first call. */
    if (memo == NULL) {
        memo_cap = 8;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return (seq_t)0;           /* out of memory – return 0 */
        for (idx_t i = 0; i < 7; ++i) memo[i] = (seq_t)0;
        memo[7] = (seq_t)1;
        memo_len = 8;
    }

    /* Return the pre‑computed value if it already exists. */
    if (n < memo_len) return memo[(size_t)n];

    /* Extend the memoisation table up to index n. */
    for (idx_t k = memo_len; k <= n; ++k) {
        /* Grow the array if necessary. */
        if (k >= memo_cap) {
            idx_t new_cap = memo_cap * 2;
            seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failed */
            memo = tmp;
            memo_cap = new_cap;
        }
        /* Compute the next Octanacci number as the sum of the previous 8. */
        seq_t sum = memo[k-1] + memo[k-2] + memo[k-3] + memo[k-4]
                  + memo[k-5] + memo[k-6] + memo[k-7] + memo[k-8];
        memo[k] = sum;
    }
    memo_len = n + 1;   /* we have now stored up to index n */

    return memo[(size_t)n];
}
