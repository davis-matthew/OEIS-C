/*
 * A162460  First differences of A161762.
 * https://oeis.org/A162460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 5*a(n-3) - a(n-4) + a(n-5) for n >=
 *            6; with initial values a(1) = 0, a(2) = 2, a(3) = 2, a(4) = 7,
 *            a(5) = 14
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Return 0 for indices below the offset – behaviour is irrelevant. */
    if (n < 1) return 0;

    /*--- static memoisation storage ---------------------------------*/
    /* memo[i] holds a(i) for i >= 1.  memo[0] is unused. */
    static seq_t *memo = NULL;          /* array of computed terms   */
    static size_t memo_cap = 0;         /* current allocated size   */
    static idx_t memo_max = 0;          /* largest index computed   */

    /* Expand storage if necessary */
    if ((size_t)n + 1 > memo_cap) {
        size_t new_cap = memo_cap ? memo_cap * 2 : 8;
        if ((size_t)n + 1 > new_cap) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed – abort with zero value */
            return 0;
        }
        memo = tmp;
        memo_cap = new_cap;
        /* initialise newly allocated slots to 0 – not strictly required */
        for (size_t i = memo_max + 1; i < new_cap; ++i) memo[i] = 0;
    }

    /* Compute terms up to n if not already done */
    for (idx_t i = memo_max + 1; i <= n; ++i) {
        if (i == 1) {
            memo[1] = (seq_t)0;
        } else if (i == 2) {
            memo[2] = (seq_t)2;
        } else if (i == 3) {
            memo[3] = (seq_t)2;
        } else if (i == 4) {
            memo[4] = (seq_t)7;
        } else if (i == 5) {
            memo[5] = (seq_t)14;
        } else { /* recurrence for i >= 6 */
            seq_t term = (seq_t)3 * memo[i - 1]
                       + memo[i - 2]
                       - (seq_t)5 * memo[i - 3]
                       - memo[i - 4]
                       + memo[i - 5];
            memo[i] = term;
        }
    }
    memo_max = n;
    return memo[n];
}
