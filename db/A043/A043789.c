/*
 * A043789  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 4 mod 6.
 * https://oeis.org/A043789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose base‑3 representation has a number
 *            of runs congruent to 4 modulo 6. Indexing starts at 1
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* for exit */
#include <stdbool.h>  /* bool */

static seq_t *cache = NULL;   /* cached terms, 0‑based index */
static idx_t cache_len = 0;   /* number of cached terms */
static idx_t cache_cap = 0;   /* capacity of cache */

/* Ensure the cache can hold at least `min_cap` terms.  */
static void grow_cache(idx_t min_cap) {
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)min_cap) new_cap = (size_t)min_cap;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);               /* fatal on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Count the number of runs in the base‑3 representation of x.
   A run is a maximal block of equal digits.  */
static int runs_in_base3(seq_t x) {
    int runs = 0;
    int prev = -1;
    while (x > 0) {
        seq_t r = x % 3;
        int digit = (int)r;            /* 0, 1 or 2 – safe cast */
        if (digit != prev) {
            ++runs;
            prev = digit;
        }
        x /= 3;
    }
    return runs;
}

/* Return the nth term of A043789 (n ≥ 1).  */
seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined for offset 1 */

    /* Generate terms until we have at least n. */
    if (cache_len < n) {
        /* First value to test: one greater than the last cached value. */
        seq_t start = cache_len ? cache[cache_len - 1] + 1 : 1;
        seq_t cur   = start;

        while (cache_len < n) {
            if (runs_in_base3(cur) % 6 == 4) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = cur;
            }
            cur += 1;
        }
    }
    return cache[n - 1];
}
