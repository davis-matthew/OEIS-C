/*
 * A223258  Rolling icosahedron footprints: number of n X 3 0..11 arrays
 *          starting with 0 where 0..11 label vertices of an icosahedron and
 *          every array movement to a horizontal or vertical neighbor moves
 *          along an icosahedral edge.
 * https://oeis.org/A223258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 38*a(n-1) - 129*a(n-2), with initial values a(1)=25,
 *            a(2)=845
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   max_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/max_idx.h"

/* Compute a(n) for n >= 1. Parameters seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for n < 1 */

    /* memo stores values 1..memo_count; index 0 unused */
    static seq_t *memo = NULL;
    static idx_t memo_count = 0;            /* highest n for which memo[n] is valid */

    /* initial allocation when memo is empty */
    if (memo == NULL) {
        /* need space at least up to index 2 */
        size_t need = (n > 2 ? (size_t)n + 1 : 3);
        memo = (seq_t *)malloc(need * sizeof(seq_t));
        if (!memo) return (seq_t)0;         /* allocation failure */

        memo[1] = (seq_t)25;
        memo[2] = (seq_t)845;
        memo_count = 2;
    }
    /* grow array if new n exceeds current capacity */
    else if (memo_count < n) {
        size_t need = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, need * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        memo = tmp;
    }

    /* If n is 1 or 2, we can return immediately */
    if (n == 1) return memo[1];
    if (n == 2) return memo[2];

    /* Compute any missing values up to n, using the recurrence only for k >= 3 */
    idx_t start = max_idx(memo_count + 1, (idx_t)3);
    for (idx_t k = start; k <= n; ++k) {
        /* recurrence: a(k) = 38*a(k-1) - 129*a(k-2) */
        seq_t term = (seq_t)38 * memo[k - 1] - (seq_t)129 * memo[k - 2];
        memo[k] = term;
    }
    memo_count = n;                  /* now valid up to n */

    return memo[n];
}
