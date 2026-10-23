/*
 * A029776  Digits of n appear in n^3.
 * https://oeis.org/A029776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A029776  
   a(n) is the n‑th non‑negative integer k such that
   every decimal digit appearing in k also appears in k³.  
   The implementation searches sequentially, caching all found
   values so that repeated calls are fast.  Larger values may be
   represented when the build uses a wider seq_t. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Test whether the property holds for a given number.  The candidate
   is passed as an idx_t so we can examine arbitrarily large values
   while storing the result in seq_t.  The cube is computed in
   unsigned __int128 to avoid overflow for 64‑bit builds. */
static bool has_property(idx_t num)
{
    /* Compute k³ in a wide type. */
    unsigned __int128 cube = (unsigned __int128)num * num * num;

    /* Mark which digits appear in the cube. */
    bool seen[10] = {false};
    unsigned __int128 t = cube;
    if (t == 0) {
        seen[0] = true;
    } else {
        while (t > 0) {
            seen[(int)(t % 10)] = true;
            t /= 10;
        }
    }

    /* Check that every digit of num is present in the cube. */
    idx_t x = num;
    if (x == 0) {
        return seen[0];
    }
    while (x > 0) {
        int d = (int)(x % 10);
        if (!seen[d]) return false;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Ensure n is at least 1 (the sequence starts at offset 1). */
    if (n < 1) return (seq_t)0;

    /* Static cache of found values. */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  count  = 0;
    static idx_t   next   = 0;      /* next candidate to test */

    /* Allocate or grow the cache to accommodate the requested index. */
    if (cache == NULL) {
        cap = (size_t)n > 32 ? (size_t)n : 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    } else if (cap < (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (tmp == NULL) abort();   /* allocation failure */
        cache = tmp;
        cap = (size_t)n;
    }

    /* Generate terms until we have at least n of them. */
    while (count < (size_t)n) {
        if (has_property(next)) {
            cache[count++] = (seq_t)next;
        }
        next = next + (idx_t)1;
    }

    return cache[n - 1];
}
