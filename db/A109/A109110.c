/*
 * A109110  a(n) = 2a(n-1) + a(n-2) - a(n-3); a(0)=4, a(1)=9, a(2)=20.
 * https://oeis.org/A109110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3); a(0)=4, a(1)=9, a(2)=20
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Stores the already-computed terms.  The array grows as needed. */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;   /* Number of terms currently stored   */
    static idx_t memo_cap  = 0;   /* Allocated capacity                 */

    /* Initialisation */
    if (memo == NULL) {
        memo_cap = 4;
        memo = alloc_seq_array(memo_cap);
        memo[0] = (seq_t)4;
        memo[1] = (seq_t)9;
        memo[2] = (seq_t)20;
        memo_size = 3;
    }

    /* For n that we already have return immediately */
    if (n < memo_size)
        return memo[n];

    /* Extend memo if necessary before each write */
    for (idx_t i = memo_size; i <= n; ++i) {
        if (i >= memo_cap) {
            /* Double the capacity */
            idx_t new_cap = memo_cap * 2;
            seq_t *tmp = alloc_seq_array(new_cap);
            for (idx_t j = 0; j < memo_cap; ++j)
                tmp[j] = memo[j];
            free_seq_array(memo);
            memo = tmp;
            memo_cap = new_cap;
        }
        /* Compute a(i) via the recurrence */
        seq_t val = 2 * memo[i - 1] + memo[i - 2] - memo[i - 3];
        memo[i] = val;
    }

    memo_size = n + 1;
    return memo[n];
}
