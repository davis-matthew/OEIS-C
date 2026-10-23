/*
 * A043708  Numbers whose base-5 representation has an odd number of runs.
 * https://oeis.org/A043708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A043708 – numbers whose base‑5 representation has an odd number
 *            of runs
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Test if a number in base 5 has an odd number of runs (changes of digit). */
static bool odd_runs(seq_t x)
{
    const seq_t five = 5;
    int last_digit = -1;
    unsigned runs = 0;

    while (x > 0) {
        int digit = (int)(x % five);
        if (digit != last_digit) {
            runs++;
            last_digit = digit;
        }
        x /= five;
    }
    return (runs & 1) == 1;
}

/* Return the n‑th term of A043708 (1‑based).  The table is built lazily. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static size_t cached_size = 0;
    static size_t cache_cap   = 0;
    static seq_t last_checked = 0; /* largest integer already examined */

    if ((size_t)n <= cached_size)          /* already computed */
        return cache[n - 1];

    /* Extend the cache until it contains at least n terms. */
    while (cached_size < (size_t)n) {
        last_checked += 1;
        if (odd_runs(last_checked)) {
            if (cached_size >= cache_cap) {   /* grow allocation */
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cached_size] = last_checked;   /* store new term */
            cached_size += 1;
        }
    }
    return cache[n - 1];
}
