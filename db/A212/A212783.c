/*
 * A212783  Half the number of 0..n arrays of length 4 with second differences
 *          nonzero.
 * https://oeis.org/A212783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 3*a(n-3) - a(n-4) + a(n-5) + 3*a(n-6)
 *            - a(n-7) - 2*a(n-8) + a(n-9) Initial values: a(1)=5, a(2)=27,
 *            a(3)=99, a(4)=252, a(5)=546, a(6)=1034, a(7)=1803, a(8)=2925,
 *            a(9)=4517
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[10] = {
    0, /* dummy for 0‑indexing */
    5, 27, 99, 252, 546, 1034, 1803, 2925, 4517
};

static seq_t *cache = NULL;    /* 1‑based cache */
static size_t cache_len = 0;   /* largest n cached (>=0) */

static void ensure_cache(idx_t n)
{
    /* No work needed for n < 1 */
    if (n < 1) return;

    /* Initialise cache with the first 9 terms if necessary */
    if (cache == NULL) {
        cache_len = 9;                     /* we have a(1)..a(9) ready */
        cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        for (size_t i = 1; i <= cache_len; ++i)
            cache[i] = init_vals[i];
    }

    /* If n already cached, nothing to do */
    if ((size_t)n <= cache_len)
        return;

    /* Grow array to hold up to a(n) */
    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
    if (tmp == NULL) {               /* allocation failed – abort */
        abort();
    }
    cache = tmp;

    /* Compute missing terms (start after what we already have) */
    for (size_t i = cache_len + 1; i <= new_len; ++i) {
        seq_t pos = ((seq_t)2) * cache[i - 1]
                    + cache[i - 2]
                    + cache[i - 5]
                    + ((seq_t)3) * cache[i - 6]
                    + cache[i - 9];

        seq_t neg = ((seq_t)3) * cache[i - 3]
                    + cache[i - 4]
                    + cache[i - 7]
                    + ((seq_t)2) * cache[i - 8];

        cache[i] = pos - neg;
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1 */

    ensure_cache(n);
    return cache[(size_t)n];
}
