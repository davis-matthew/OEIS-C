/*
 * A194406  Numbers m such that Sum_{k=1..m} (<1/2 + k*e> - <k*e>) = 0 where <
 *          > denotes fractional part.
 * https://oeis.org/A194406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer m satisfying Sum_{k=1..m} (
 *            fractional_part(k*e+1/2) - fractional_part(k*e) ) = 0 (OEIS
 *            sequence A194406)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Precomputed constants: e ≈ 2.7182818284590452353602874713526… */
    static unsigned __int128 E = 0;          /* e * 10^30  */
    static unsigned __int128 SCALE = 0;      /* 10^30      */
    static unsigned __int128 HALF = 0;       /* SCALE/2    */
    static bool init = false;

    if (!init) {
        /* Build E from decimal string */
        const char *e_str = "2718281828459045235602874713526";
        unsigned __int128 val = 0;
        for (; *e_str; ++e_str)
            val = val * 10 + (unsigned __int128)(*e_str - '0');
        E = val;

        /* Build SCALE = 10^30 */
        unsigned __int128 s = 1;
        for (int i = 0; i < 30; ++i) s *= 10;
        SCALE = s;
        HALF = s / 2;
        init = true;
    }

    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_k = 1;   /* next k to evaluate */
    static long long diff = 0; /* (#terms with f<0.5) - (#with f≥0.5) */

    /* Grow the cache until we have at least n terms */
    while (cache_len < n) {
        /* Evaluate k = next_k */
        unsigned __int128 prod = (unsigned __int128)next_k * E;
        unsigned __int128 f1 = prod / SCALE;                 /* floor(k*e) */
        unsigned __int128 f2 = (prod + HALF) / SCALE;        /* floor(k*e+0.5) */

        if (f1 == f2)      /* fractional part < 0.5 */
            ++diff;
        else
            --diff;

        /* If diff is zero and k is even, we have a term */
        if (diff == 0 && (next_k & 1ULL) == 0) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
