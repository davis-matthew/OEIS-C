/*
 * A224556  Number of (n+2) X 7 0..1 matrices with each 3 X 3 subblock
 *          idempotent.
 * https://oeis.org/A224556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 4*a(n-4) + 3*a(n-5) -
 *            a(n-6) (valid for n > 8, with a(1)=263, a(2)=246, a(3)=320,
 *            a(4)=395, a(5)=540, a(6)=740, a(7)=988, a(8)=1351)
 * Range:     u8 none, u16 n <= 18, u32 n <= 47, u64 n <= 105, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* memoisation array, 1‑based index */
static size_t cache_cap = 0;    /* current capacity of cache */
static size_t cache_next = 0;   /* first index that is not yet computed */

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1 by the caller */
    size_t idx = (size_t)n;  /* cast to size_t for array indexing */

    /* enlarge cache if needed */
    if (!cache || idx > cache_cap) {
        size_t new_cap = idx * 2 + 10;   /* grow heuristically */
        seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!new_cache)
            abort();        /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise the first eight values once */
    if (cache_next == 0) {
        cache[1] = 263;
        cache[2] = 246;
        cache[3] = 320;
        cache[4] = 395;
        cache[5] = 540;
        cache[6] = 740;
        cache[7] = 988;
        cache[8] = 1351;
        cache_next = 9;   /* next index to compute */
    }

    /* compute missing values up to idx */
    while (cache_next <= idx) {
        /* use the recurrence:
           a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 4*a(n-4) + 3*a(n-5) - a(n-6) */
        seq_t val = (seq_t)4 * cache[cache_next - 1];
        val -= (seq_t)6 * cache[cache_next - 2];
        val += (seq_t)5 * cache[cache_next - 3];
        val -= (seq_t)4 * cache[cache_next - 4];
        val += (seq_t)3 * cache[cache_next - 5];
        val -= cache[cache_next - 6];

        cache[cache_next] = val;
        ++cache_next;
    }

    return cache[idx];
}
