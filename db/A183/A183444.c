/*
 * A183444  Number of n X 3 binary arrays with every 1 having exactly two
 *          king-move neighbors equal to 1.
 * https://oeis.org/A183444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 8*a(n-3) - 7*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) for n > 6; initial values: a(1)=1, a(2)=9, a(3)=18,
 *            a(4)=30, a(5)=107, a(6)=265
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based cache of results */
static size_t cache_cap = 0;  /* allocated capacity */
static idx_t  max_comp  = 0;  /* maximum n already computed */

/* Ensure the cache is large enough to hold entries up to n (1‑based). */
static void
ensure_cache_size(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)(n + 1))
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();                       /* unrecoverable allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n=0; return 0 */

    ensure_cache_size(n);

    if (n > max_comp) {
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)
                val = 1;
            else if (i == 2)
                val = 9;
            else if (i == 3)
                val = 18;
            else if (i == 4)
                val = 30;
            else if (i == 5)
                val = 107;
            else if (i == 6)
                val = 265;
            else
                val = ((seq_t)2) * cache[i-1]
                     - cache[i-2]
                     + ((seq_t)8) * cache[i-3]
                     - ((seq_t)7) * cache[i-4]
                     + ((seq_t)2) * cache[i-5]
                     - ((seq_t)2) * cache[i-6];
            cache[i] = val;
        }
        max_comp = n;
    }
    return cache[n];
}
