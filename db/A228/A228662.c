/*
 * A228662  Number of 3 X n binary arrays with top left value 1 and no two
 *          ones adjacent horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - 5*a(n-3), * with a(1)=4, a(2)=5,
 *            a(3)=34. The recurrence holds for all n≥4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* This function works both for scalar seq_t types and for
   arbitrary‑precision integer classes.  The only restriction is
   that seq_t must be default constructible and assignable. */
seq_t a(idx_t n)
{
    /* 10001 elements comfortably cover the desired range (≈10 000). */
    static seq_t   cache[10001];
    static bool    computed[10001] = {false};

    if (n == 0)                /* sub‑offset term is undefined */
        return (seq_t)0;

    if (n < 10001) {
        /* Initialise known values on first call. */
        if (!computed[1]) {
            cache[1] = (seq_t)4;  computed[1] = true;
            cache[2] = (seq_t)5;  computed[2] = true;
            cache[3] = (seq_t)34; computed[3] = true;
        }

        /* Compute lazily up to n. */
        for (idx_t i = 4; i <= n; ++i) {
            if (!computed[i]) {
                cache[i] = 2 * cache[i - 1]
                         + 6 * cache[i - 2]
                         - 5 * cache[i - 3];
                computed[i] = true;
            }
        }
        return cache[n];
    }

    /* If n exceeds the preallocated range, fall back to a dynamic cache.
       The dynamic cache grows in powers of two and always keeps the
       computed values.  This code is safe for scalar types and for
       arbitrary‑precision classes because it uses malloc/free and
       assigns to the objects rather than relying on realloc. */
    static seq_t *big_cache = NULL;
    static idx_t  big_size  = 0;

    if (n + 1 > big_size) {
        idx_t new_size = (big_size == 0) ? 1024 : big_size;
        while (new_size <= n)
            new_size *= 2;

        seq_t *tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        /* Initialise new entries to their default value via assignment. */
        for (idx_t i = 0; i < new_size; ++i)
            tmp[i] = (seq_t)0;

        /* Copy any values already stored in the static cache. */
        idx_t copy_len = (big_size < 10001) ? big_size : 10001;
        for (idx_t i = 0; i < copy_len; ++i)
            tmp[i] = cache[i];

        free(big_cache);
        big_cache = tmp;
        big_size  = new_size;
    }

    /* Compute values up to n in the dynamic cache. */
    for (idx_t i = 4; i <= n; ++i) {
        if (big_cache[i] == (seq_t)0) {
            big_cache[i] = 2 * big_cache[i - 1]
                         + 6 * big_cache[i - 2]
                         - 5 * big_cache[i - 3];
        }
    }
    return big_cache[n];
}
