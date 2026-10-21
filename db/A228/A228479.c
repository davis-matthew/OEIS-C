/*
 * A228479  Number of n X 5 binary arrays with top left value 1 and no two
 *          ones adjacent horizontally, vertically or antidiagonally.
 * https://oeis.org/A228479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 15*a(n-2) + 16*a(n-3) * - 11*a(n-4) -
 *            20*a(n-5) + 19*a(n-6) * - 8*a(n-7) + a(n-9) * Initial values
 *            (offset 1): a(1)=5, a(2)=19, a(3)=127, a(4)=728, * a(5)=4354,
 *            a(6)=25699, a(7)=152373, a(8)=902042, a(9)=5342712. * This
 *            implementation memoises the sequence up to the largest n re...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_values[9] = {
    (seq_t)5,      (seq_t)19,      (seq_t)127,
    (seq_t)728,    (seq_t)4354,    (seq_t)25699,
    (seq_t)152373, (seq_t)902042,  (seq_t)5342712
};

static seq_t *cache = NULL;      /* array of computed terms */
static idx_t  cached_len = 0;    /* number of terms currently stored (a(1)...a(cached_len)) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the sequence is defined for n ≥ 1 */

    /* Return known values directly for n ≤ 9. */
    if (n <= 9u) return init_values[n-1u];

    /* Allocate or grow the cache as needed. */
    if (cache == NULL || cached_len < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – return 0 as a safe sentinel */
        cache = tmp;

        /* Write the initial 9 terms if they have not been cached yet. */
        for (idx_t i = cached_len; i < 9u && i < n; ++i) {
            cache[i] = init_values[i];
        }

        /* Compute any remaining terms up to n using the recurrence. */
        idx_t start = (cached_len > 9u ? cached_len : 9u);
        for (idx_t i = start; i < n; ++i) {
            seq_t term;

            /* The recurrence uses a(i-1), a(i-2), …, a(i-9).  */
            term = (seq_t)3 * cache[i-1u];
            term += (seq_t)15 * cache[i-2u];
            term += (seq_t)16 * cache[i-3u];
            term -= (seq_t)11 * cache[i-4u];
            term -= (seq_t)20 * cache[i-5u];
            term += (seq_t)19 * cache[i-6u];
            term -= (seq_t)8  * cache[i-7u];
            term +=          cache[i-9u];

            cache[i] = term;
        }

        cached_len = n;
    }

    return cache[n-1u];
}
