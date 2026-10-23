/*
 * A022907  The sequence m(n) in A022905.
 * https://oeis.org/A022907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * A033485(n-1) - 1, n > 1 (with a(1) = 0) A033485
 *            satisfies the recurrence: b[1] = 1 b[n] = b[n-1] + b[floor(n/2)]
 *            for n >= 2 Thus a(n) can be computed in O(n) time
 * Range:     u8 n <= 16, u16 n <= 72, u32 n <= 529, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset handling: a(1) = 0 */
    if (n <= 1) return 0;

    /* we need values of b up to index (n-1) */
    idx_t m = n - 1;                     /* m >= 1 */
    /* allocate array b[0..m], using 0 as dummy */
    seq_t *b = (seq_t*)malloc((m + 1) * sizeof(seq_t));
    if (!b) return 0;                    /* malloc failure: return 0 */

    b[0] = 0;                            /* unused but safe */
    b[1] = 1;                            /* base case */

    for (idx_t i = 2; i <= m; ++i) {
        b[i] = b[i-1] + b[i / 2];        /* floor(i/2) is integer division */
    }

    seq_t result = (seq_t)3 * b[m] - 1;
    free(b);
    return result;
}
