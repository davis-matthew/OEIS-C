/*
 * A186817  Half the number of (n+2) X 3 binary arrays with each 3 X 3
 *          subblock having sum 4 or 5.
 * https://oeis.org/A186817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 9*a(n-2) + 15*a(n-3) - 36*a(n-4) - 72*a(n-5) -
 *            27*a(n-6) - 81*a(n-7)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values:
      a(1)=126, a(2)=606, a(3)=2904, a(4)=14229,
      a(5)=70767, a(6)=351549, a(7)=1750896 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed values, 1‑based */
static idx_t computed = 0;    /* highest index already computed */

static void init_cache(idx_t n)
{
    /* ensure storage for indices 0..n (size n+1) */
    idx_t new_size = n > 7 ? n + 1 : 8;   /* at least indices 0..7 */
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    /* set constants (they may overwrite if re‑expanded) */
    cache[0] = 0;
    cache[1] = 126;
    cache[2] = 606;
    cache[3] = 2904;
    cache[4] = 14229;
    cache[5] = 70767;
    cache[6] = 351549;
    cache[7] = 1750896;
    /* mark the highest computed index */
    computed = n > 7 ? 7 : n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n <= 0, return 0 */

    /* If no cache yet, create a minimal one */
    if (cache == NULL)
        init_cache(n);

    /* If requested index is beyond current cache size, grow it */
    if (n + 1 > computed + 1) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    }

    /* Compute any new terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1)       cache[i] = 126;
        else if (i == 2)  cache[i] = 606;
        else if (i == 3)  cache[i] = 2904;
        else if (i == 4)  cache[i] = 14229;
        else if (i == 5)  cache[i] = 70767;
        else if (i == 6)  cache[i] = 351549;
        else if (i == 7)  cache[i] = 1750896;
        else {
            seq_t v = (seq_t)3 * cache[i-1];
            v += (seq_t)9 * cache[i-2];
            v += (seq_t)15 * cache[i-3];
            v -= (seq_t)36 * cache[i-4];
            v -= (seq_t)72 * cache[i-5];
            v -= (seq_t)27 * cache[i-6];
            v -= (seq_t)81 * cache[i-7];
            cache[i] = v;
        }
    }
    computed = n > computed ? n : computed;
    return cache[n];
}
