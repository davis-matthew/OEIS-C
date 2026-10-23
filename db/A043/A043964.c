/*
 * A043964  Numbers k such that 0 and 5 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A043964 – numbers k such that 0 and 5 occur juxtaposed in the base‑9 representation of k but not of k+1.
   The function builds the sequence lazily and caches results for repeated calls. */
#include <stdbool.h>
#include <stdlib.h>

/* Return true if base‑9 representation of x contains adjacent digits 0 and 5 (in either order). */
static bool has_adjacent_0_5(seq_t x)
{
    if (x == 0) return false;          /* base‑9 "0" has no adjacency */
    const seq_t base = (seq_t)9;
    seq_t prev = x % base;             /* least significant digit */
    x /= base;
    while (x != 0) {
        seq_t cur = x % base;
        if ((prev == 0 && cur == 5) || (prev == 5 && cur == 0))
            return true;
        prev = cur;
        x /= base;
    }
    return false;
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n < 1, but harmless */

    /* Static cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* number of terms stored */
    static size_t cache_capacity = 0;  /* allocated capacity */
    static seq_t next_k = 0;           /* next integer to test */

    /* Expand cache until we have at least n terms */
    while (cache_size < n) {
        /* Find next term */
        while (true) {
            if (has_adjacent_0_5(next_k) && !has_adjacent_0_5(next_k + 1)) {
                /* Allocate or grow cache */
                if (cache_size >= cache_capacity) {
                    size_t new_cap = (cache_capacity == 0) ? 64 : cache_capacity * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) { /* allocation failed */ return 0; }
                    cache = tmp;
                    cache_capacity = new_cap;
                }
                cache[cache_size++] = next_k;
                break;
            }
            ++next_k;
        }
        ++next_k;   /* advance past the found term for the next iteration */
    }

    /* Return cached value (sequence is 1‑based) */
    return cache[n - 1];
}
