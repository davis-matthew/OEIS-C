/*
 * A043858  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 0 mod 9.
 * https://oeis.org/A043858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that number of runs in base‑4 representation of k is 0 mod 9 */
#include <stdlib.h>
#include <stdbool.h>

/* Helper to count runs in base 4 */
static int count_runs_base4(seq_t x)
{
    if (x == 0) return 1;
    int runs = 0;
    int prev = -1;
    while (x > 0) {
        int digit = (int)(x % 4);
        if (digit != prev) {
            ++runs;
            prev = digit;
        }
        x /= 4;
    }
    return runs;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    /* Ensure at least n terms are available in the cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 0;   /* next integer to test (0,1,2,...) */

    /* Lazy initialization of cache and candidate counter */
    if (cache == NULL) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) { /* memory allocation failed – abort */
            exit(EXIT_FAILURE);
        }
    }

    /* Grow cache if needed */
    if (cache_len < n) {
        /* Allocate more space in exponential steps */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL) {
                exit(EXIT_FAILURE);
            }
            cache = tmp;
        }

        /* Scan integers until we have enough terms */
        while (cache_len < n) {
            int runs = count_runs_base4(next_candidate);
            if (runs % 9 == 0) {
                cache[cache_len++] = next_candidate;
            }
            ++next_candidate;
        }
    }

    /* Return the n‑th term (1‑based index) */
    return cache[n - 1];
}
