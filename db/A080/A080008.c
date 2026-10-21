/*
 * A080008  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=2, r=3, I={-1,0}.
 * https://oeis.org/A080008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-3)+2*a(n-4)+2*a(n-5)-a(n-7)-a(n-9)-a(n-10)
 *            initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=2, a(5)=2,
 *            a(6)=3, a(7)=5, a(8)=11, a(9)=15
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static memoized table */
    static seq_t *memo = NULL;
    static size_t memo_cap = 0;
    static idx_t memo_len = 0;

    /* base values up to a(9) */
    static const seq_t base[10] = {
        1ULL, 0ULL, 0ULL, 1ULL, 2ULL, 2ULL, 3ULL, 5ULL, 11ULL, 15ULL
    };

    /* allocate initial storage if needed */
    if (!memo) {
        memo_cap = 64;
        memo = (seq_t*)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return 0; /* failure: return 0, unlikely on OJ */
        for (idx_t i = 0; i < memo_cap; ++i) memo[i] = 0;
        memo_len = 0;
    }

    /* grow memo array if the requested index is beyond current capacity */
    while (n >= memo_cap) {
        size_t new_cap = memo_cap << 1;
        seq_t *tmp = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        memo = tmp;
        for (size_t i = memo_cap; i < new_cap; ++i) memo[i] = 0;
        memo_cap = new_cap;
    }

    /* compute values up to n */
    while (memo_len <= n) {
        seq_t val;
        if (memo_len < 10) {
            val = base[memo_len];
        } else {
            val = memo[memo_len - 2]
                + memo[memo_len - 3]
                + (seq_t)2 * memo[memo_len - 4]
                + (seq_t)2 * memo[memo_len - 5]
                - memo[memo_len - 7]
                - memo[memo_len - 9]
                - memo[memo_len - 10];
        }
        memo[memo_len++] = val;
    }

    return memo[n];
}
