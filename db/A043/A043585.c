/*
 * A043585  Numbers whose base-3 representation has exactly 5 runs.
 * https://oeis.org/A043585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Return the n‑th positive integer whose base‑3
 * representation has exactly five runs.  The routine builds a
 * cache of all such numbers found so far; on each call it
 * generates candidates until it has at least n terms, then
 * returns the (n‑1)th cached value.  This works for any
 * seq_t (8–128‑bit unsigned or arbitrary precision) and for
 * any idx_t (unsigned 64‑bit). */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* array of found numbers   */
    static size_t capacity = 0;       /* allocated size          */
    static size_t count = 0;          /* number stored so far    */
    static idx_t  next = 1;           /* next integer to test    */

    /* initial allocation */
    if (cache == NULL) {
        capacity = 16;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();
    }

    /* generate candidates until we have at least n terms */
    while (count < (size_t)n) {
        seq_t x = next;
        unsigned runs = 0;
        unsigned prev = 3;     /* sentinel greater than any base‑3 digit */

        /* count runs in the base‑3 expansion of x */
        while (x > 0) {
            unsigned d = (unsigned)(x % 3);
            if (d != prev) {
                ++runs;
                prev = d;
            }
            x /= 3;
        }

        if (runs == 5) {
            /* grow cache if necessary */
            if (count == capacity) {
                size_t new_cap = capacity * 2;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                capacity = new_cap;
            }
            cache[count++] = next;
        }
        ++next;
    }

    return cache[(size_t)(n - 1)];
}
