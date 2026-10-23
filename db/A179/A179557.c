/*
 * A179557  a(1)=10; a(n) = a(n-1)*10 - (-1)^n.
 * https://oeis.org/A179557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * 10 - (-1)^n with a(1) = 10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Computes the n-th term of OEIS A179557.
 * Supports arbitrary‑precision seq_t (or native unsigned types).
 * Implements an O(n) recurrence with memoization. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for zero or negative n */

    /* static cache of computed terms */
    static seq_t *cache = NULL;   /* cache[1] .. cache[max_computed]   */
    static idx_t cache_size = 0;  /* allocated size (elements)         */
    static idx_t max_computed = 0;

    /* enlarge cache if needed (indices 0..n inclusive) */
    if (cache_size <= n) {
        idx_t new_size = n + 1;                     /* include index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                        /* allocation failure – return 0 */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)10;
        } else {
            seq_t prev = cache[i - 1] * (seq_t)10;
            /* (-1)^i : +1 for odd i, -1 for even i */
            if ((i & 1u) == 0)          /* i even */
                cache[i] = prev - (seq_t)1;
            else                         /* i odd */
                cache[i] = prev + (seq_t)1;
        }
    }
    max_computed = n;
    return cache[n];
}
