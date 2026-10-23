/*
 * A043809  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 3 mod 9.
 * https://oeis.org/A043809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043809: Numbers n such that the number of runs in the base‑3
 * representation of n is congruent to 3 modulo 9. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Count the number of runs in the base‑3 representation of x. */
static inline idx_t runs_in_base3(idx_t x)
{
    idx_t runs = 0;
    int prev = -1;                /* sentinel for "no previous digit" */
    while (x > 0) {
        int digit = (int)(x % 3);
        if (digit != prev) {
            ++runs;
            prev = digit;
        }
        x /= 3;
    }
    return runs;
}

/* Memoised generator of the sequence.  Each new index causes the function
 * to extend its internal cache until the requested term is available. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_candidate = 1;   /* smallest number yet to be examined */

    /* Ensure the cache holds n terms. */
    while (cache_len < n) {
        /* Grow the cache array if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* In a real application we would handle out‑of‑memory
                 * appropriately.  Here we just abort. */
                abort();
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Scan candidates for valid terms until we reach the desired length. */
        while (cache_len < n) {
            if (runs_in_base3(next_candidate) % 9 == 3) {
                cache[cache_len++] = (seq_t)next_candidate;
            }
            ++next_candidate;
        }
    }

    /* Return the requested term (1‑based index). */
    return cache[n - 1];
}
