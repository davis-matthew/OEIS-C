/*
 * A039351  Numbers whose base-8 representation has the same nonzero number of
 *          4's and 5's.
 * https://oeis.org/A039351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑8 representation has the same nonzero
 * number of 4's and 5's.  The function uses a brute‑force search, caching
 * results so repeated calls are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Cache for found terms */
static seq_t *seq_cache = NULL;   /* array of found terms */
static size_t seq_len   = 0;      /* number of cached terms    */
static size_t seq_cap   = 0;      /* current capacity of cache */
static idx_t current    = 1;      /* next integer to test */

/* Return true iff x has equal non‑zero counts of 4 and 5 in base‑8. */
static bool has_equal_45(seq_t x)
{
    unsigned int cnt4 = 0, cnt5 = 0;
    while (x != 0) {
        int d = (int)(x % 8);          /* digit 0–7 */
        if (d == 4) cnt4++;
        else if (d == 5) cnt5++;
        x /= 8;
    }
    return (cnt4 == cnt5) && (cnt4 > 0);
}

/* Ensure cache has room for at least one more element. */
static void ensure_cache_capacity(void)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
        void *tmp = realloc(seq_cache, new_cap * sizeof(seq_t));
#ifdef __cplusplus
        seq_cache = reinterpret_cast<seq_t*>(tmp);
#else
        seq_cache = (seq_t*)tmp;
#endif
        seq_cap   = new_cap;
    }
}

/* Return the n‑th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Extend cache until we have at least n elements. */
    while (seq_len < n) {
        /* Find next integer with equal non‑zero 4's and 5's. */
        while (!has_equal_45((seq_t)current))
            ++current;
        ensure_cache_capacity();
        seq_cache[seq_len++] = (seq_t)current;
        ++current;            /* next integer to test */
    }

    return seq_cache[n - 1];
}
