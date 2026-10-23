/*
 * A033905  Sort then Add, a(1)=31.
 * https://oeis.org/A033905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=31; for n>1, a(n)=a(n-1)+sortdigits(a(n-1)) * (sortdigits
 *            sorts the decimal digits of the argument in ascending order *
 *            and interprets the result as an integer)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 56, u64 n <= 345,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Internal helper: return the digits of x sorted in ascending order
   and formed back into a number. */
static seq_t sort_ascending(seq_t x)
{
    unsigned int cnt[10] = {0};
    seq_t t = x;

    if (t == (seq_t)0) {
        cnt[0] = 1;
    } else {
        while (t != (seq_t)0) {
            int d = (int)(t % (seq_t)10);
            cnt[d]++;
            t = t / (seq_t)10;
        }
    }

    seq_t res = (seq_t)0;
    for (int d = 0; d < 10; ++d) {
        for (unsigned int i = 0; i < cnt[d]; ++i)
            res = res * (seq_t)10 + (seq_t)d;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;          /* safety for out‑of‑bounds */

    static seq_t *cache = NULL;                  /* array indexed by n (1..size) */
    static idx_t cache_size = 0;                 /* largest n computed so far */

    if (n > cache_size) {
        /* Allocate a fresh array large enough for indices 0..n */
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;          /* out-of-memory guard */

        /* Copy already cached values, if any */
        if (cache_size > 0)
            memcpy(new_cache, cache, (cache_size + 1) * sizeof(seq_t));

        /* Initialise a(1) when a fresh array is created */
        if (n >= (idx_t)1 && cache_size == (idx_t)0)
            new_cache[1] = (seq_t)31;

        /* Compute terms from the next missing index up to n */
        idx_t start = (cache_size == (idx_t)0) ? (idx_t)2 : (cache_size + (idx_t)1);
        for (idx_t i = start; i <= n; ++i) {
            seq_t prev   = new_cache[i - (idx_t)1];
            seq_t sorted = sort_ascending(prev);
            new_cache[i] = prev + sorted;
        }

        /* Replace old cache */
        free(cache);
        cache = new_cache;
        cache_size = n;
    }

    /* Return the requested term */
    return cache[n];
}
