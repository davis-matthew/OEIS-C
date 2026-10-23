/*
 * A159722  Number of permutations of 3 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 2 local maxima.
 * https://oeis.org/A159722
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 52*a(n-1) - 928*a(n-2) + 6784*a(n-3) - 21760*a(n-4) +
 *            25600*a(n-5) for n>6 Initial values: a(2)=12, a(3)=576,
 *            a(4)=17376, a(5)=448800, a(6)=10861056, a(7)=253940736
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache indexed from n=2 */
static size_t   cache_len   = 0;   /* number of elements stored         */
static size_t   cache_cap   = 0;   /* allocated capacity                */
static const idx_t cache_start = 2; /* index corresponding to seq_cache[0] */

seq_t a(idx_t n)
{
    if (n < cache_start)
        return 0;                     /* undefined for n < 2 */

    idx_t idx = n - cache_start;
    size_t need = (size_t)idx + 1;

    /* grow cache if necessary */
    if (need > cache_cap) {
        size_t new_cap = need > 32 ? need : 32;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)
            abort();
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new entries */
    for (size_t i = cache_len; i < need; ++i) {
        idx_t cur = (idx_t)(i + cache_start);
        if (cur == 2)
            seq_cache[i] = (seq_t)12;
        else if (cur == 3)
            seq_cache[i] = (seq_t)576;
        else if (cur == 4)
            seq_cache[i] = (seq_t)17376;
        else if (cur == 5)
            seq_cache[i] = (seq_t)448800;
        else if (cur == 6)
            seq_cache[i] = (seq_t)10861056;
        else if (cur == 7)
            seq_cache[i] = (seq_t)253940736;
        else {  /* recurrence for cur >= 8 */
            seq_t a1 = seq_cache[i-1];
            seq_t a2 = seq_cache[i-2];
            seq_t a3 = seq_cache[i-3];
            seq_t a4 = seq_cache[i-4];
            seq_t a5 = seq_cache[i-5];
            seq_cache[i] = (seq_t)52 * a1
                         - (seq_t)928 * a2
                         + (seq_t)6784 * a3
                         - (seq_t)21760 * a4
                         + (seq_t)25600 * a5;
        }
    }

    cache_len = need;
    return seq_cache[idx];
}
