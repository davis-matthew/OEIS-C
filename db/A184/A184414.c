/*
 * A184414  Upper s(n)-Wythoff sequence, where s(n)=floor[(n+1)/2].
 * https://oeis.org/A184414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Upper s(n)-Wythoff sequence on-line construction.
   s(n) = floor((n+1)/2).  For each n, pick the smallest unused
   integer L, then take U = L + s(n); mark both L and U as used
   and store U as a(n).  The table is built incrementally and
   memoised so that repeated calls are O(1). */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for n < 1 */

    /* Static state for incremental construction */
    static idx_t gen_n      = 0;     /* number of terms built */
    static idx_t next_cand  = 1;     /* next integer to try for L */
    static unsigned char *used = NULL;   /* bitmap of used integers */
    static size_t used_cap  = 0;
    static seq_t *cache = NULL;         /* a(n) values */
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Grow the cache until it contains the requested term */
    while (gen_n < n)
    {
        /* Ensure `used` can hold `next_cand` */
        if ((size_t)next_cand >= used_cap) {
            size_t new_cap = used_cap ? used_cap * 2 : 64;
            while (new_cap <= (size_t)next_cand) new_cap <<= 1;
            unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
            if (!tmp) abort();
            memset(tmp + used_cap, 0, new_cap - used_cap);
            used     = tmp;
            used_cap = new_cap;
        }

        /* Find the smallest unused integer for this n */
        while ((size_t)next_cand < used_cap && used[next_cand]) ++next_cand;
        idx_t L = next_cand;   /* lower term for this index */

        /* Compute s(n) = floor((gen_n + 1 + 1)/2) = floor((gen_n+2)/2) */
        idx_t s = (gen_n + 2) / 2;
        idx_t U = L + s;      /* upper term for this index */

        /* Ensure `used` can hold `U` */
        if ((size_t)U >= used_cap) {
            size_t new_cap = used_cap ? used_cap * 2 : 64;
            while (new_cap <= (size_t)U) new_cap <<= 1;
            unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
            if (!tmp) abort();
            memset(tmp + used_cap, 0, new_cap - used_cap);
            used     = tmp;
            used_cap = new_cap;
        }

        /* Mark L and U as used */
        used[L] = 1;
        used[U] = 1;

        /* Store the upper term in the cache */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache     = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)U;

        ++gen_n;   /* move to next index */
    }

    return cache[n - 1];
}
