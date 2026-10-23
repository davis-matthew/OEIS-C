/*
 * A103210  a(n) = (1/n) * Sum_{i=0..n-1} C(n,i)*C(n,i+1)*2^i*3^(n-i), a(0)=1.
 * https://oeis.org/A103210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2 * Sum_{k=0}^{n-1} a(k)*a(n-1-k), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;      /* cached sequence values */
static idx_t   cache_len = 0;        /* number of elements currently stored */

/* compute a(n) using recurrence with memoization (O(n^2) overall) */
seq_t a(idx_t n)
{
    if (n + 1 > cache_len) {
        /* need more space */
        void *tmp = realloc(seq_cache, sizeof(seq_t) * (n + 1));
        if (!tmp) abort();              /* allocation failure */
        seq_cache = (seq_t *)tmp;

        /* initialize cache if empty */
        if (cache_len == 0) {
            seq_cache[0] = (seq_t)1;   /* a(0) = 1 */
            cache_len = 1;
        }

        /* compute new terms from cache_len to n inclusive */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t total = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                total += seq_cache[k] * seq_cache[i - 1 - k];
            }
            seq_cache[i] = seq_cache[i - 1] + (seq_t)2 * total;
        }
        cache_len = n + 1;
    }
    return seq_cache[n];
}
