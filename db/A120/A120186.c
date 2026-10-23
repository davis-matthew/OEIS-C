/*
 * A120186  a(n) = ceiling( Sum_{i=1..n-1} a(i)/7 ), a(1) = 1.
 * https://oeis.org/A120186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(n)=ceil( Sum_{i=1}^{n-1} a(i) / 7 )
 * Range:     u8 n <= 46, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static idx_t computed_max = 0;
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static unsigned __int128 sum128 = 0;

static void grow_cache(idx_t n)
{
    if (n <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 1;
    while (newcap < (size_t)n) newcap <<= 1;
    seq_t *newp = (seq_t*)malloc(newcap * sizeof(seq_t));
    if (!newp) abort();
    if (cache) {
        memcpy(newp, cache, computed_max * sizeof(seq_t));
        free(cache);
    }
    cache = newp;
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* safety, offset starts at 1 */

    if (n > computed_max) {
        grow_cache(n);
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t ai;
            if (i == 1) {
                ai = 1;
                sum128 = 1;
            } else {
                ai = (seq_t)((sum128 + 6) / 7);   /* ceil division */
                sum128 += (unsigned __int128)ai;
            }
            cache[i - 1] = ai;
        }
        computed_max = n;
    }
    return cache[n - 1];
}
