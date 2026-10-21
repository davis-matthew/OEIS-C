/*
 * A050035  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 2 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = a(2) = 1 and a(3) = 3.
 * https://oeis.org/A050035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, a(3)=3; for n >= 4: a(n) = a(n-1) + a(m) where m
 *            = 2*n - 2 - 2^(p+1) and p is the unique integer with 2^p < n-1
 *            <= 2^(p+1). Computed with memoisation
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 126, u64 n <= 505,
 *            u128 n <= 3085, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, exit */

/* Cache for previously computed values.  Using seq_t (unsigned) as null
   is not a valid sequence value, so zero can be used as sentinel. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can store values up to n (inclusive). */
static void ensure_cache_cap(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap <= (size_t)n) new_cap = n + 1;

    seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newptr) exit(EXIT_FAILURE);

    /* Zero the newly allocated portion. */
    if (newptr != cache) {
        memset(newptr + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache = newptr;
    } else {
        memset(cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    }
    cache_cap = new_cap;
}

/* Recursive helper with memoisation. */
static seq_t get_a(idx_t n)
{
    if (n <= 3) {
        switch ((int)n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)1;
            case 3: return (seq_t)3;
        }
    }

    ensure_cache_cap(n);
    if (cache[n] != 0) return cache[n];

    /* a(n-1) */
    seq_t prev = get_a(n - 1);

    /* Find p such that 2^p < n-1 <= 2^(p+1) */
    idx_t k = n - 1;          /* k = n-1 >= 3 for n>=4 */
    idx_t next_pow = 2;       /* 2^(p+1) starts at 2 for p=0 */
    while (next_pow < k) {
        next_pow <<= 1;       /* move to next power of two (2^(p+1)) */
    }
    /* Now next_pow == 2^(p+1) satisfying 2^p < k <= 2^(p+1) */

    idx_t m = 2 * n - 2 - next_pow;   /*  m < n  */

    seq_t m_val = get_a(m);

    seq_t res = prev + m_val;
    cache[n] = res;
    return res;
}

/* Public function with external linkage. */
seq_t a(idx_t n)
{
    return get_a(n);
}
