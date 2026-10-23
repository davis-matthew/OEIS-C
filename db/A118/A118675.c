/*
 * A118675  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+47)^2 = y^2.
 * https://oeis.org/A118675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by recurrence a(1)=0, a(2)=16, a(3)=85, a(4)=141,
 *            a(5)=225, a(6)=616 a(n) = 6*a(n-3) - a(n-6) + 94 for n > 6
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed values, 0‑based */
static size_t cache_len = 0;         /* number of elements currently stored */
static size_t cache_cap = 0;         /* capacity of the array */

/* initial values a(1)..a(6) mapped to indices 0..5 */
static const seq_t init_vals[6] = {
    0,    /* a(1) */
    16,   /* a(2) */
    85,   /* a(3) */
    141,  /* a(4) */
    225,  /* a(5) */
    616   /* a(6) */
};

/* expand the cache to hold at least 'new_len' elements */
static void grow_cache_size(size_t new_len) {
    if (new_len <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < new_len)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort(); /* unrecoverable allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* fill the cache up to index n-1 (i.e., n elements) */
static void ensure_cache(idx_t n) {
    if (n == 0)
        return;
    size_t target = (size_t)n;           /* number of elements needed */
    if (cache_len >= target)
        return;

    grow_cache_size(target);

    size_t i = cache_len;
    /* fill any missing initial values up to index 5 */
    if (i < 6) {
        const size_t upto = target < 6 ? target : 6;
        for (; i < upto; ++i)
            cache[i] = init_vals[i];
        i = upto;
    }

    /* compute remaining values using the recurrence */
    for (; i < target; ++i) {
        /* a(i+1) = 6*a(i-2) - a(i-5) + 94  (indices are 0‑based) */
        seq_t val = 6 * cache[i-3] - cache[i-6] + 94;
        cache[i] = val;
    }

    cache_len = target;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;  /* undefined for n<1, but safe fallback */
    ensure_cache(n);
    return cache[n-1];
}
