/*
 * A170661  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   For 1 ≤ n ≤ 48:      a(n) = 26 * Σ_{k=0}^{n-1} a(k) + 2
   For n = 49:          a(49) = 26 * Σ_{k=0}^{48} a(k) + 1
   For n ≥ 50:          a(n) = 26 * Σ_{k=1}^{48} a(n-k) − 351 * a(n‑49)
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;
    static size_t  cache_len = 0;   /* number of terms stored = last_index+1 */

    /* keep total prefix sum and sliding window sum (size up to 48) */
    static seq_t pref_sum = 0;
    static seq_t win_sum  = 0;      /* sum of last min(48, i) terms ending at i-1 */

    /* also remember highest n computed so far */
    static idx_t  known_n = 0;

    /* allocate initial capacity if needed */
    if (cache_len == 0) {
        size_t init_cap = 64;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        cache_len = 0;
    }

    /* ensure cache large enough for requested n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* should never happen in grade‑school code */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first term if we haven't yet */
    if (known_n == 0) {
        cache[0] = (seq_t)1;
        pref_sum = (seq_t)1;
        win_sum  = (seq_t)1;          /* window of last 1 term */
        cache_len = 1;
        known_n = 0;
    }

    /* if requested n already computed, return it */
    if (n < known_n) return cache[n];

    /* compute terms from known_n+1 up to n */
    for (idx_t i = known_n + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 48) {
            val = (seq_t)26 * pref_sum + (seq_t)2;
        } else if (i == 49) {
            val = (seq_t)26 * pref_sum + (seq_t)1;
        } else { /* i >= 50 */
            val = (seq_t)26 * win_sum - (seq_t)351 * cache[i - 49];
        }

        cache[i] = val;

        /* update running sums for next iterations */
        pref_sum += val;
        win_sum  += val;
        if (i >= 48) {                 /* remove the term that falls out of the last‑48 window */
            win_sum -= cache[i - 48];
        }
    }

    known_n   = n;
    cache_len = n + 1;

    return cache[n];
}
