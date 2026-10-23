/*
 * A044577  Numbers n such that string 1,1 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044577: numbers n such that the base‑8 representation of n
   contains the substring "11" but the base‑8 representation of n+1 does not.
   The function `a(n)` returns the nth such number (offset 1). */
#include <stdbool.h>
#include <stdlib.h>

/* Check whether the base‑8 representation of x contains the substring 11 */
static bool has11(seq_t x)
{
    if (x == 0)
        return false;
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 8);
        if (d == 1 && prev == 1)
            return true;
        prev = d;
        x = x / 8;
    }
    return false;
}

/* Return the nth element (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Persistent cache: store the seen terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of cache array */
    static idx_t cache_count = 0;  /* number of terms currently stored */
    static seq_t next_candidate = 1; /* next number to test */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* allocation failed; return 0 as a safe default */
    }

    /* Generate terms until we have at least n of them. */
    while (cache_count < n) {
        seq_t y = next_candidate;

        /* Find next y that satisfies the property. */
        while (!(has11(y) && !has11(y + 1))) {
            y = y + 1;
        }

        /* Store y in cache, expanding if necessary. */
        if (cache_count >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp != NULL) {
                cache = tmp;
                cache_cap = new_cap;
            } else {
                /* Allocation failed: keep old buffer but stop growth. */
            }
        }
        cache[cache_count] = y;
        cache_count++;

        next_candidate = y + 1;
    }

    return cache[(size_t)(n - 1)];
}
