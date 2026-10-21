/*
 * A073932  Define f(n) = n - largest nontrivial divisor of n or f(n) = n-1 if
 *          n is a prime [that is, f(n) = A060681(n)]. Form a triangle in
 *          which the n-th row contains terms n, f(n), f(f(n)), ... until a 1
 *          is reached; sequence gives triangle read by rows.
 * https://oeis.org/A073932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = read across rows of a triangle defined by f(n) = n -
 *            largest nontrivial divisor of n (or n-1 if n is prime). The
 *            function builds the sequence lazily, caching all computed terms
 *            so that subsequent calls reuse earlier work
 * Range:     u8 n <= 2463, u16 n <= 8949, u32 n <= 8949, u64 n <= 8949,
 *            u128 n <= 8949, big any n
 * Helpers:   f_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/f_func.h"

/* external function requested by the problem description */
seq_t a(idx_t n) {
    if (n == 0)          /* safety; not used as offset is 1 */
        return (seq_t)0;

    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_start = 1;    /* next starting integer to process */

    /* grow cache until it holds at least n terms */
    while (cache_len < n) {
        seq_t val = (seq_t)next_start;   /* start new row with this integer */
        for (;;) {
            /* grow array if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failure – abort */
                    exit(1);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;

            if (val == 1)
                break;                /* end of this row */

            val = f_func(val);        /* next term in the row */
        }
        ++next_start;                  /* prepare for the following row */
    }

    return cache[n - 1];                /* sequence is 1‑indexed */
}
