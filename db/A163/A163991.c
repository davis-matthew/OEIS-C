/*
 * A163991  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A163991
    a(0) = 1
    a(1) = 23
    a(2) = 506
    a(3) = 11132
    a(4) = 244904
    a(5) = 5387888
    a(6) = 118533283
    a(7) = 2607726660
    for n ≥ 8:
        a(n) = 22*a(n‑1) – 252*a(n‑6) + 231*a(n‑7)
    (Derived from the generating function
     (1+x)*(1‑x⁶)/(1‑22x+252x⁶‑231x⁷).)
    The recurrence uses the non‑homogeneous terms
    u₁ = 1,  u₆ = –1,  u₇ = –1; all other uₙ = 0.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;   /* number of elements stored (≥1 after init) */

    /* initialise cache with a(0) if needed */
    if (cache == NULL) {
        cache_len = (idx_t)(n + 1);
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        /* compute terms up to n */
        for (idx_t i = 1; i <= n; ++i) {
            seq_t val = (seq_t)22 * cache[i - 1];
            if (i >= 6) val -= (seq_t)252 * cache[i - 6];
            if (i >= 7) val += (seq_t)231 * cache[i - 7];
            if (i == 1)      val += (seq_t)1;
            else if (i == 6) val -= (seq_t)1;
            else if (i == 7) val -= (seq_t)1;
            cache[i] = val;
        }
        return cache[n];
    }

    /* extend cache if necessary */
    if (n + 1 > cache_len) {
        idx_t old_len = cache_len;
        cache_len = (idx_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)cache_len * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t val = (seq_t)22 * cache[i - 1];
            if (i >= 6) val -= (seq_t)252 * cache[i - 6];
            if (i >= 7) val += (seq_t)231 * cache[i - 7];
            if (i == 1)      val += (seq_t)1;
            else if (i == 6) val -= (seq_t)1;
            else if (i == 7) val -= (seq_t)1;
            cache[i] = val;
        }
    }

    return cache[n];
}
