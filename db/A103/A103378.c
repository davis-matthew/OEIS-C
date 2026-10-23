/*
 * A103378  a(n) = a(n-10) + a(n-11) for n > 11, and a(n) = 1 for 1 <= n <=
 *          11.
 * https://oeis.org/A103378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-10) + a(n-11) for n > 11, a(n) = 1 for 1 ≤ n ≤ 11
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

/*
 * Computes the nth term of OEIS sequence A103378.
 *
 * The function uses memoisation: an array of previously computed
 * values is grown as needed.  The array is indexed by the sequence
 * index directly, so memo[1] holds a(1).  Index 0 is unused.
 *
 * All arithmetic is performed in seq_t, which can be an unsigned
 * integer of various widths or an arbitrary‑precision type.  The
 * functions seq_malloc and seq_realloc handle construction for any
 * such type, so this code works correctly in both C and C++ builds.
 */
seq_t a(idx_t n)
{
    /* Static memoisation table */
    static seq_t *memo = NULL;          /* pointer to array of seq_t */
    static idx_t memo_size = 0;         /* current allocated size (next free slot) */

    /* Base case: first 11 terms are 1 */
    if (n <= 11)
        return (seq_t)1;

    /* Grow memoisation table if needed */
    if (n >= memo_size) {
        idx_t new_cap = n + 1;          /* we need indices 0..n */

        if (memo == NULL) {
            memo = seq_malloc(new_cap);
        } else {
            memo = seq_realloc(memo, new_cap);
        }

        /* Initialise newly allocated entries */
        for (idx_t i = memo_size; i <= n; ++i) {
            if (i <= 11) {
                memo[i] = (seq_t)1;     /* should not happen for i>11 but safe */
            } else {
                memo[i] = memo[i - 10] + memo[i - 11];
            }
        }
        memo_size = new_cap;           /* update allocated size */
    }

    return memo[n];
}
