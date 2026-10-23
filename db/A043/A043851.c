/*
 * A043851  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 1 mod 8.
 * https://oeis.org/A043851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043851: numbers k such that the number of runs in the base‑4 representation of k is congruent to 1 (mod 8). */
#include <stdlib.h>

/* Count runs in base‑4 representation of x */
static int count_runs(seq_t x) {
    if (x == (seq_t)0) return 1;          /* representation of 0 is "0", one run */
    int runs = 0;
    int prev = -1;
    while (x > (seq_t)0) {
        int d = (int)(x % 4);            /* current least‑significant digit */
        if (d != prev) {
            ++runs;
            prev = d;
        }
        x = x / 4;                       /* shift right by one digit */
    }
    return runs;
}

/* Return the n‑th element (1‑based) of A043851 */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_k = 0;              /* last integer examined */

    while (cache_len < (size_t)n) {
        ++last_k;                          /* next integer to test */
        if (count_runs(last_k) % 8 == 1) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_k;
        }
    }
    return cache[(size_t)n - 1];
}
