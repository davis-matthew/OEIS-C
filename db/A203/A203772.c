/*
 * A203772  a(n)=f(a(n-1),a(n-2)+1,a(n-3)+1), where f(x,y,z)=yz+zx+xy and
 *          (a(1),a(2),a(3))=(0,0,1).
 * https://oeis.org/A203772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A203772:
 *  Recurrence:  a(1)=0, a(2)=0, a(3)=1,
 *  a(n)=f(a(n-1),a(n-2)+1,a(n-3)+1)   for n≥4,
 *  where f(x,y,z)=yz+zx+xy. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = 0;

static void ensure_cache(idx_t n)
{
    if (n <= last_computed)
        return;

    /* Allocate enough space for indices 0 … n (index 0 unused). */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp)
            abort();
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the initial values the first time they are needed. */
    if (last_computed < 3) {
        seq_cache[1] = (seq_t)0;   /* a(1) */
        seq_cache[2] = (seq_t)0;   /* a(2) */
        seq_cache[3] = (seq_t)1;   /* a(3) */
        last_computed = 3;
    }

    /* Compute any missing terms up to index n. */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        seq_t x = seq_cache[k - 1];
        seq_t y = seq_cache[k - 2] + (seq_t)1;
        seq_t z = seq_cache[k - 3] + (seq_t)1;
        seq_cache[k] = y * z + z * x + x * y;
    }

    last_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* indices below the offset are undefined */
    ensure_cache(n);
    return seq_cache[n];
}
