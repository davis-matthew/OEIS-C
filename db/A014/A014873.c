/*
 * A014873  Numbers k that divide s(k), where s(1)=1, s(j)=22*s(j-1)+j.
 * https://oeis.org/A014873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014873: a(n) = nth k such that k divides s(k),
   with s(1)=1 and s(j)=22*s(j-1)+j. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* External integer types supplied by the build system. */
extern seq_t a(idx_t);

/* Cache for already found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity */
static idx_t last_k_tested = 0; /* largest k that has been examined */

/* Ensure cache has at least needed capacity. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < cache_len + 1)
        new_cap = cache_len + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Main function requested by the template. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n=0, but harmless */

    /* Extend cache until it holds at least n terms. */
    while (cache_len < n) {
        /* Next candidate k. */
        ++last_k_tested;
        idx_t k = last_k_tested;

        /* Compute s(k) modulo k using the recurrence. */
        idx_t s_mod = 1 % k;    /* s(1)=1, start for modulo k */
        for (idx_t j = 2; j <= k; ++j) {
            unsigned __int128 temp = (unsigned __int128)22 * s_mod + j;
            s_mod = (idx_t)(temp % k);
        }

        if (s_mod == 0) {           /* k divides s(k) */
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
