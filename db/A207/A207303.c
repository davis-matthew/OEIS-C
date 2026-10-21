/*
 * A207303  Number of n X 6 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) for n>6 Initial terms: a(1)=19, a(2)=361, a(3)=1675,
 *            a(4)=4939, a(5)=11497, a(6)=23091
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 102, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;

    size_t new_size = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();          /* allocation failed */
    cache = new_cache;

    for (size_t i = cache_size; i < new_size; ++i) {
        if (i < 6) {
            /* initialise the known first six terms */
            switch (i) {
                case 0: cache[i] = (seq_t)19;      break;
                case 1: cache[i] = (seq_t)361;     break;
                case 2: cache[i] = (seq_t)1675;    break;
                case 3: cache[i] = (seq_t)4939;    break;
                case 4: cache[i] = (seq_t)11497;   break;
                case 5: cache[i] = (seq_t)23091;   break;
            }
        } else {
            /* use the recurrence relation for n>6 */
            seq_t term = 6 * cache[i-1];
            term    = term - 15 * cache[i-2];
            term    = term + 20 * cache[i-3];
            term    = term - 15 * cache[i-4];
            term    = term + 6 * cache[i-5];
            term    = term - cache[i-6];
            cache[i] = term;
        }
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out-of-range index */
    ensure_cache(n);
    return cache[n-1];
}
