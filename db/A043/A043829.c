/*
 * A043829  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 1 mod 4.
 * https://oeis.org/A043829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the number of runs in the base‑4 representation
 * of k is congruent to 1 (mod 4).  The function returns the nth term of this
 * sequence (a(1) is the first listed value). */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for already generated terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* number of terms stored                    */
    static size_t cache_cap  = 0;      /* allocated capacity of cache array         */
    static seq_t last_k = 0;           /* last integer that has been examined       */

    /* Expand cache memory if needed */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed – return 0 as safe value   */
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        /* Examine next integer */
        last_k += 1;

        /* Count runs in the base‑4 representation of last_k */
        int runs = 0;
        seq_t x = last_k;
        int prev_digit = 4;   /* sentinel larger than any base‑4 digit */
        while (x > 0) {
            int d = (int)(x % 4);
            if (d != prev_digit) {
                ++runs;
                prev_digit = d;
            }
            x /= 4;
        }

        /* Check the congruence condition */
        if (runs % 4 == 1) {
            /* Ensure cache has space for a new element */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;  /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_k;   /* store the qualifying number */
        }
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}
