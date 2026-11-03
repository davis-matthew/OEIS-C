/*
 * A043972  Numbers k such that 1 and 6 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A043972: numbers k whose base‑9 representation contains two
 * adjacent digits 1 and 6 in either order, while k + 1 does not.  The
 * function searches integers sequentially and caches found terms. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Return true if the base‑9 representation of x contains an adjacent pair
 * of digits 1 and 6 (either 16 or 61). */
static bool has_adjacent_1and6(seq_t x)
{
    int prev = -1;  /* previous LSB digit */
    while (x != 0) {
        int d = (int)(x % 9);
        if ((prev == 1 && d == 6) || (prev == 6 && d == 1))
            return true;
        prev = d;
        x /= 9;
    }
    return false;
}

/* Return the n‑th term (n ≥ 1) of A043972. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1; behaviour for 0 is irrelevant */

    static seq_t *cache = NULL;        /* array of found terms */
    static idx_t size      = 0;        /* number of cached terms */
    static size_t capacity = 0;        /* allocated capacity of cache */
    static seq_t last_k = 0;           /* last term found, start next search after it */

    /* Grow the cache until we have at least n terms. */
    while (size < n) {
        seq_t k = last_k + 1;          /* next candidate */
        for (;;) {
            if (has_adjacent_1and6(k) && !has_adjacent_1and6(k + 1)) {
                /* Ensure capacity */
                if (size == capacity) {
                    size_t new_cap = capacity ? capacity * 2 : 16;
                    seq_t *new_cache = seq_realloc(cache, capacity, new_cap);
                    cache = new_cache;
                    capacity = new_cap;
                }
                cache[size++] = k;
                last_k = k;
                break;
            }
            ++k;
        }
    }

    return cache[n - 1];
}
