/*
 * A168681  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^17 = I.
 * https://oeis.org/A168681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 4 a(n) = 3*a(n-1) for 2 ≤ n ≤ 16 a(17) = 3*a(16)
 *            - 5*a(0) - 1 a(18) = 3*a(17) - 5*a(1) + 3*a(0) - 1 a(n) =
 *            3*a(n-1) - 5*a(n-17) + 3*a(n-18) for n ≥ 19
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* never include a main() function */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache array holding a(n) values                */
static size_t cache_len = 0;  /* number of elements currently stored (size)    */

/* ensure that a[i] is computed for all 0 ≤ i ≤ n */
static void
ensure_cache(idx_t n)
{
    const size_t target = (size_t)n + 1;          /* we need indices 0…n inclusive      */
    if (cache_len >= target)                     /* already have them                 */
        return;

    /* grow the cache array if needed */
    seq_t *new_cache = (seq_t *)realloc(cache, target * sizeof(seq_t));
    if (!new_cache)                               /* allocation failure                */
        abort();

    cache = new_cache;

    /* compute new terms starting from the first missing index */
    for (size_t i = cache_len; i < target; ++i) {
        seq_t term;
        if (i == 0) {
            term = (seq_t)1;                      /* a(0) = 1                         */
        } else if (i == 1) {
            term = (seq_t)4;                      /* a(1) = 4                         */
        } else if (i <= 16) {
            /* for 2 ≤ i ≤ 16: a(i) = 3*a(i-1)     */
            term = (seq_t)3 * cache[i - 1];
        } else if (i == 17) {
            /* a(17) = 3*a(16) - 5*a(0) - 1       */
            term = (seq_t)3 * cache[16]
                 - (seq_t)5 * cache[0]
                 - (seq_t)1;
        } else if (i == 18) {
            /* a(18) = 3*a(17) - 5*a(1) + 3*a(0) - 1 */
            term = (seq_t)3 * cache[17]
                 - (seq_t)5 * cache[1]
                 + (seq_t)3 * cache[0]
                 - (seq_t)1;
        } else {
            /* recurrence for n ≥ 19: a(n) = 3*a(n-1) - 5*a(n-17) + 3*a(n-18) */
            term = (seq_t)3 * cache[i - 1]
                 - (seq_t)5 * cache[i - 17]
                 + (seq_t)3 * cache[i - 18];
        }
        cache[i] = term;
    }
    cache_len = target;
}

seq_t a(idx_t n)
{
    /* caller guarantees n ≥ 0; compute up to n if necessary */
    ensure_cache(n);
    return cache[(size_t)n];
}
