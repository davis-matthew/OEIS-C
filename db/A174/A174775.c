/*
 * A174775  y-values in the solution to x^2 - 37*y^2 = 1.
 * https://oeis.org/A174775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 146*a(n-1)-a(n-2) with a(1)=0, a(2)=12
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_seq, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cached_max = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but safe */
    if (!cache) {                  /* first time: initialise cache */
        cache_cap = 3;             /* reserve indices 0,1,2 */
        cache = alloc_seq(cache_cap);
        cache[0] = 0;              /* unused slot */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)12;
        cached_max = 2;
    }
    if (n <= cached_max) {
        return cache[n];
    }
    if ((size_t)(n + 1) > cache_cap) {
        grow_seq(&cache, &cache_cap, (size_t)(n + 1));
    }
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        cache[i] = ((seq_t)146) * cache[i - 1] - cache[i - 2];
    }
    cached_max = n;
    return cache[n];
}
