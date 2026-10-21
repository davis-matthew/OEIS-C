/*
 * A202640  Number of (n+2) X 3 binary arrays avoiding patterns 000 and 111 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            2*a(n-1)+3*a(n-2)-a(n-3)-5*a(n-4)-2*a(n-5)+5*a(n-6)+a(n-7)-2*a(n-8)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 19, u64 n <= 41, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values (offset 1): 66, 172, 462, 1278, 3448, 9394, 25526, 69336, 188418   */

#include <stdlib.h>

static const seq_t init[9] = {
    (seq_t)66,  (seq_t)172,  (seq_t)462,
    (seq_t)1278, (seq_t)3448, (seq_t)9394,
    (seq_t)25526, (seq_t)69336, (seq_t)188418
};

static seq_t *cache = NULL;   /* cache[1..N]   */
static idx_t  cache_len = 0;  /* largest index computed */

/* Ensure that cache contains terms up to index n (n >= 10) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;                /* already computed */

    /* grow storage */
    size_t new_len = (size_t)(n + 1);           /* include index 0 unused */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();                         /* out of memory */
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init[i-1];
        } else {
            /* compute via recurrence, grouping positives and negatives
               to avoid intermediate wrap‑around with unsigned arithmetic.
               The recurrence always yields a non‑negative result. */
            seq_t pos = 2 * cache[i-1] + 3 * cache[i-2]
                       + 5 * cache[i-6] + cache[i-7];
            seq_t neg = cache[i-3] + 5 * cache[i-4]
                       + 2 * cache[i-5] + 2 * cache[i-8];
            cache[i] = pos - neg;
        }
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0 */
    if (n <= 9) return init[n-1];
    ensure_cache(n);
    return cache[n];
}
