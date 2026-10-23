/*
 * A123136  a(n+1) = ( a(n) + n*(n+1) ) / n, with a(1) = 208.
 * https://oeis.org/A123136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=208, a(n+1) = ( a(n) + n*(n+1) ) / n for n>=1
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] = a(1) */
static idx_t  cache_size = 0; /* number of entries currently cached */

/* compute a(n) for n>=1, memoised */
seq_t a(idx_t n)
{
    if (n == 0)               /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    if (cache_size < n) {
        /* enlarge cache to hold up to n elements */
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* out of memory: return 0 as fallback */
        cache = tmp;

        /* compute any new terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) { /* i is 1‑based index of a(i) */
            if (i == 1) {
                cache[0] = (seq_t)208;               /* a(1) = 208 */
            } else {
                idx_t m = i - 1;                      /* n in the recurrence */
                seq_t n_seq = (seq_t)m;
                seq_t term   = n_seq * (n_seq + (seq_t)1);
                seq_t numerator = cache[i - 2] + term;
                cache[i - 1] = numerator / (seq_t)m;   /* integer division */
            }
        }
        cache_size = n;
    }
    return cache[n - 1];
}
