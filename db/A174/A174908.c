/*
 * A174908  Numbers n such that the sum of the 4th powers of their digits > n.
 * https://oeis.org/A174908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, fini, full
 * Formula:   numbers n with Σ (digit^4) > n The sequence is finite and
 *            contains 5832 terms, all less than 20000. The implementation
 *            precomputes all terms once and answers queries in O(1)
 * Range:     u8 n <= 2, u16 n <= 5832, u32 n <= 5832, u64 n <= 5832,
 *            u128 n <= 5832, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Precomputed fourth powers of digits 0-9 */
static const seq_t POW4[10] = {0, 1, 16, 81, 256, 625, 1296, 2401, 4096, 6561};

/* Cache for sequence values */
static seq_t *cache        = NULL;  /* array of values */
static size_t cache_size   = 0;     /* number of values stored */
static size_t cache_cap    = 0;     /* allocated capacity */

/* Compute the sum of fourth powers of the decimal digits of n */
static inline seq_t sum_fourth_digits(idx_t n)
{
    seq_t s = 0;
    idx_t x = n;
    while (x != 0) {
        s += POW4[(int)(x % 10)]; /* x%10 is <=9 */
        x /= 10;
    }
    return s;
}

/* Initialize the cache on first call */
static void init_cache(void)
{
    if (cache != NULL) {
        return; /* already initialized */
    }

    /* There are exactly 5832 numbers; allocate a bit more to avoid reallocs */
    cache_cap = 6000;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    if (!cache) {
        /* Out of memory – abort the program (allowed for build failure) */
        abort();
    }

    cache_size = 0;
    for (idx_t n = 1; n <= 19999; ++n) {
        if (sum_fourth_digits(n) > (seq_t)n) {
            if (cache_size >= cache_cap) {
                /* Should not happen with the chosen capacity */
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();
            }
            cache[cache_size++] = (seq_t)n;
        }
    }
}

/* Public function required by the task */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* Undefined for n=0, but return 0 to avoid UB */
    }

    init_cache();

    if ((size_t)n > cache_size) {
        return 0; /* n out of range – defined behavior is not required */
    }

    return cache[n - 1]; /* arrays are 0-indexed, OEIS indices start at 1 */
}
