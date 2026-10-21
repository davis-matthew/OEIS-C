/*
 * A211276  a(n) = number of n-lettered words in the alphabet {1, 2, 3} with
 *          as many occurrences of the substring (consecutive subword) [1, 1]
 *          as of [1, 2].
 * https://oeis.org/A211276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n-letter words over the alphabet {1,2,3} with
 *            as many occurrences of the consecutive subwords [1,1] as of
 *            [1,2]. The function implements a dynamic‑programming algorithm
 *            that keeps track of the last symbol and the difference d = (# of
 *            [1,1]) – (# of [1,2]). Only words with d = 0 a...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Base case: the empty word is counted once. */
    if (n == 0U)
        return (seq_t)1;

    /* Integer offset to map the difference d ∈ [‑n, +n] to an array
       index.  The array size must accommodate the full range for the
       largest possible n. */
    const idx_t off = n;
    const idx_t sz  = 2 * n + 1;          /* number of different d values */

    /* Allocate six arrays: three current state arrays (ending in 1,2,3)
       and three temporary arrays for the next length.  The calloc call
       is cast to silence the C++ conversion warning. */
    seq_t *cur1 = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *cur2 = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *cur3 = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *nxt1 = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *nxt2 = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *nxt3 = (seq_t *)calloc(sz, sizeof(seq_t));

    if (!cur1 || !cur2 || !cur3 || !nxt1 || !nxt2 || !nxt3)
        return (seq_t)0;          /* allocation failed – unlikely */

    /* Length‑1 words: every ending character gives difference d = 0. */
    cur1[off] = (seq_t)1;   /* word ending with 1 */
    cur2[off] = (seq_t)1;   /* word ending with 2 */
    cur3[off] = (seq_t)1;   /* word ending with 3 */

    /* Transition loop: grow the words from length 1 up to n. */
    for (idx_t len = 1; len < n; ++len) {
        /* Clear the next‑state arrays. */
        memset(nxt1, 0, sz * sizeof(seq_t));
        memset(nxt2, 0, sz * sizeof(seq_t));
        memset(nxt3, 0, sz * sizeof(seq_t));

        /* Active range of differences for the current length. */
        const idx_t start = off - len;
        const idx_t end   = off + len;

        for (idx_t i = start; i <= end; ++i) {
            /* Transition from words ending in 1. */
            seq_t v1 = cur1[i];
            if (v1) {
                /* Append 1 → d increases by 1, new last = 1 */
                nxt1[i + 1] += v1;
                /* Append 2 → d decreases by 1, new last = 2 */
                nxt2[i - 1] += v1;
                /* Append 3 → d unchanged, new last = 3 */
                nxt3[i]     += v1;
            }

            /* Transition from words ending in 2. */
            seq_t v2 = cur2[i];
            if (v2) {
                /* Appending any symbol does not change d. */
                nxt1[i] += v2;
                nxt2[i] += v2;
                nxt3[i] += v2;
            }

            /* Transition from words ending in 3. */
            seq_t v3 = cur3[i];
            if (v3) {
                nxt1[i] += v3;
                nxt2[i] += v3;
                nxt3[i] += v3;
            }
        }

        /* Prepare for the next iteration by swapping pointers. */
        seq_t *tmp;
        tmp = cur1; cur1 = nxt1; nxt1 = tmp;
        tmp = cur2; cur2 = nxt2; nxt2 = tmp;
        tmp = cur3; cur3 = nxt3; nxt3 = tmp;
    }

    /* Result: sum of words of length n whose difference d = 0. */
    seq_t result = cur1[off] + cur2[off] + cur3[off];

    /* Free temporary memory. */
    free(cur1); free(cur2); free(cur3);
    free(nxt1); free(nxt2); free(nxt3);

    return result;
}
