/*
 * A080678  Rooted at a(0)=0 and a(1)=a(2)=a(3)=1, 4 cases of index mod 4:
 *          a(4n)=4*a(n), a(4n+1)= 3*a(n)+a(n+1), a(4n+2) = 2*a(n)+2*a(n+1),
 *          and a(4n+3) = a(n)+3*a(n+1).
 * https://oeis.org/A080678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080678
   a(0) = 0, a(1) = a(2) = a(3) = 1
   a(4n)   = 4*a(n)
   a(4n+1) = 3*a(n) + a(n+1)
   a(4n+2) = 2*a(n) + 2*a(n+1)
   a(4n+3) = a(n) + 3*a(n+1)
*/

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* cached values */
static bool *computed = NULL;        /* flags whether an entry is computed */
static idx_t cache_capacity = 0;     /* allocated size */

/* Expand the cache to hold at least 'needed'+1 entries (indices 0..needed) */
static void ensure_cache(idx_t needed)
{
    if (needed < cache_capacity)
        return;
    idx_t cap = cache_capacity ? cache_capacity * 2 : 4;
    while (cap <= needed)
        cap <<= 1;

    seq_t *tmp_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    bool *tmp_comp  = (bool *)realloc(computed, cap * sizeof(bool));
    if (!tmp_cache || !tmp_comp) { /* allocation failed – abort */
        exit(1);
    }
    for (idx_t i = cache_capacity; i < cap; ++i)
        tmp_comp[i] = false;

    cache = tmp_cache;
    computed = tmp_comp;
    cache_capacity = cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    if (computed[n])
        return cache[n];

    seq_t result;
    if (n == 0) {
        result = 0;
    } else if (n < 4) { /* n = 1,2,3 */
        result = 1;
    } else {
        idx_t m = n >> 2;          /* n / 4 */
        idx_t r = n & 3;           /* n % 4 */
        seq_t a_m  = a(m);
        seq_t a_m1 = a(m + 1);

        switch (r) {
        case 0: result = 4 * a_m;            break;
        case 1: result = 3 * a_m + a_m1;     break;
        case 2: result = 2 * a_m + 2 * a_m1; break;
        case 3: result = a_m + 3 * a_m1;     break;
        }
    }

    cache[n] = result;
    computed[n] = true;
    return result;
}
