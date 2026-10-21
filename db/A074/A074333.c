/*
 * A074333  a(n) = a(n-1) + a(n-1)^3 with a(0) = 1.
 * https://oeis.org/A074333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-1)^3, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the nth term of OEIS sequence A074333.  The result is memoised
 * into a static array so that calls with arbitrary order of n are fast.
 * Only operations allowed for seq_t (integer arithmetic, bitwise ops) are
 * used.  No references or other C++‑only syntax is employed. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0..cached-1] contain a(i) for i < cached */
    static idx_t cached = 0;      /* number of entries currently cached */

    if (cached <= n) {
        /* Ensure the cache is large enough */
        idx_t newsize = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failed */
        cache = tmp;

        /* Compute all missing terms */
        for (idx_t i = cached; i < newsize; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;          /* a(0) = 1 */
            } else {
                seq_t prev = cache[i - 1];
                seq_t sq   = prev * prev;     /* a(i-1)^2   */
                seq_t cube = sq * prev;       /* a(i-1)^3   */
                cache[i]   = prev + cube;     /* a(i-1) + a(i-1)^3 */
            }
        }
        cached = newsize;
    }

    return cache[n];
}
