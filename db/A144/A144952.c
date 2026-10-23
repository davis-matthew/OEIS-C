/*
 * A144952  Total walk count of molecular graphs for linear alkanes with n
 *          carbon atoms.
 * https://oeis.org/A144952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is half the total number of walks of length 1…n‑1 in the
 *            path graph on n vertices (each walk counted with direction)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 27, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* safety for n=0 */

    /* Allocate arrays indexed 1..n (0 and n+1 unused). */
    size_t sz = (size_t)n + 2;               /* allow access to cur[n+1] */
    seq_t *cur  = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!cur || !next) return 0;              /* allocation failure */

    /* Initial vector: one walk of length 0 ending at each vertex. */
    for (idx_t i = 1; i <= n; ++i) cur[i] = (seq_t)1;

    seq_t sum_cur = 0;
    for (idx_t i = 1; i <= n; ++i) sum_cur += cur[i];

    seq_t cumWalks = 0;

    for (idx_t step = 1; step <= n - 1; ++step) {
        /* Compute number of walks of this length ending at each vertex. */
        if (n >= 2) {
            next[1] = cur[2];
            next[n] = cur[n - 1];
            for (idx_t i = 2; i <= n - 1; ++i)
                next[i] = cur[i - 1] + cur[i + 1];
        } else {
            next[1] = 0;                     /* for n == 1, no internal vertices */
        }

        /* Accumulate walks of this length. */
        seq_t sum_next = 0;
        for (idx_t i = 1; i <= n; ++i) sum_next += next[i];
        cumWalks += sum_next;

        /* Prepare for next iteration. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
        sum_cur = sum_next;
    }

    /* Free the allocated arrays. */
    free(cur);
    free(next);

    /* Half of the total walks gives the desired sequence value. */
    return cumWalks / 2;
}
