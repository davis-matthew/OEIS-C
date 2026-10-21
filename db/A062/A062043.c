/*
 * A062043  Positive numbers whose product of digits is 10 times their sum.
 * https://oeis.org/A062043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Compute by brute‑force search – test each positive integer * if
 *            its digits satisfy product = 10 * sum. The first term occurs at
 *            * 459. The function builds a cache of already found terms so
 *            that * calls are efficient even if n is queried out of order
 * Range:     u8 none, u16 n <= 126, u32 n <= 500, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* cache of found terms */
static size_t cache_cap = 0;         /* allocated capacity of cache   */
static size_t cache_size = 0;        /* number of stored terms        */
static idx_t last_checked = 0;       /* last integer that was tested  */

/* Test whether an integer x satisfies product of digits = 10 * sum of digits.
 * Any digit 0 fails immediately, and the digits 5 and an even digit are
 * implicitly required because the product then contains a factor 10. */
static inline bool good_number(idx_t x)
{
    seq_t sum = 0;
    seq_t prod = 1;
    while (x > 0) {
        unsigned d = x % 10;   /* d is in 0..9 */
        if (d == 0) return false;  /* product would be 0 – impossible */
        sum += d;
        prod *= d;
        x   /= 10;
    }
    return prod == 10 * sum;
}

/* Ensure the cache can hold at least new_cap terms. */
static void ensure_cap(size_t new_cap)
{
    if (cache_cap < new_cap) {
        size_t new_cap_alloc = cache_cap ? cache_cap * 2 : 64;
        if (new_cap_alloc < new_cap) new_cap_alloc = new_cap;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap_alloc * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap_alloc;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* behaviour below offset not required */

    /* Grow cache until we have n terms */
    while (cache_size < n) {
        ++last_checked;
        if (good_number(last_checked)) {
            ensure_cap(cache_size + 1);
            cache[cache_size++] = last_checked;
        }
    }
    return cache[n - 1];
}
