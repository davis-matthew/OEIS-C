/*
 * A098941  Numbers where 2 is the only even decimal digit.
 * https://oeis.org/A098941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate integers and keep those with decimal digits
 * from the set {1,3,5,7,9,2} that contain at least one '2'.
 * The function generates terms on demand, caching previously
 * computed values so that calls in any order remain fast. */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 1; /* last candidate examined (idx_t works for all seq_t sizes) */

/* Returns non‑zero iff x consists solely of digits 1,3,5,7,9 or 2
 * and contains at least one '2'. */
static int is_allowed(seq_t x)
{
    if (x == 0) return 0;          /* 0 is disallowed */
    int has_two = 0;
    while (x > 0) {
        int d = (int)(x % (seq_t)10);
        if (d == 2) {
            has_two = 1;
        } else if ((d & 1) == 0) { /* even digit other than 2 */
            return 0;
        }
        x /= (seq_t)10;
    }
    return has_two;
}

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them */
    while ((idx_t)cache_size < n) {
        /* Move to the next integer to test */
        ++last_checked;
        if (is_allowed((seq_t)last_checked)) {
            /* Grow the cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
