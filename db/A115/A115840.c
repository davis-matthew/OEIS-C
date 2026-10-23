/*
 * A115840  Self-describing sequence. The n-th integer of the sequence
 *          indicates how many integers of the sequence are strictly < 6n.
 * https://oeis.org/A115840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 1, a(n) = the smallest integer > a(n‑1) so that a(n) = #{
 *            k : a(k) < 6n } (self‑describing sequence)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n)
{
    /* memoized storage for already computed terms (1‑based array) */
    static seq_t *seq   = NULL;   /* seq[0] unused */
    static idx_t  size  = 0;      /* highest index that has been computed */
    static idx_t  j     = 0;      /* largest index with seq[j] < current i */

    if (n == 0) return 0;          /* safety guard – not used */

    /* compute missing terms up to the requested index */
    if (size < n) {
        idx_t new_size = n + 1;    /* +1 for 1‑based indexing */
        seq_t *tmp = (seq_t *)realloc(seq, new_size * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        seq = tmp;

        /* initialise the first term if necessary */
        if (size == 0) {
            seq[0] = 0;           /* unused slot */
            seq[1] = 1;
            size = 1;
            j    = 1;             /* a[1] < 2, so j=1 when i=2 */
        }

        for (idx_t i = size + 1; i <= n; ++i) {
            /* update j: largest index with seq[j] < i  */
            while (j + 1 <= i - 1 && (idx_t)seq[j + 1] < i) ++j;

            idx_t bound     = 6 * j;               /* derived lower bound */
            idx_t candidate = (idx_t)seq[i - 1] + 1;/* next integer > a[i-1] */

            if (candidate < bound) candidate = bound;
            seq[i] = (seq_t)candidate;
        }
        size = n;
    }
    return seq[n];
}
