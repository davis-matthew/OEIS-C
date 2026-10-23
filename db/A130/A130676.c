/*
 * A130676  Numbers n for which either 16*n^2-6*n+1 or 16*n^2-10*n-1 or both
 *          is/are prime.
 * https://oeis.org/A130676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A130676
 *   a(n) is the nth integer n (1‑based) such that
 *   at least one of 16*n^2-6*n+1 or 16*n^2-10*n-1 is prime.
 *   Equivalent to: include n if isPrimeGeneric(16*n^2-6*n+1) OR
 *   isPrimeGeneric(16*n^2-10*n-1).
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Static cache of the sequence values (the n's that satisfy the condition). */
    static seq_t *cache = NULL;       /* pointer to array of seq_t */
    static size_t cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;      /* allocated capacity */
    static idx_t last_checked = 0;    /* last n that was examined */

    if (n == 0)          /* offset is 1 – undefined for 0 */
        return (seq_t)0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        ++last_checked;   /* next candidate n */

        /* Compute the two quadratic forms using seq_t arithmetic. */
        seq_t t1 = (seq_t)16 * (seq_t)last_checked * (seq_t)last_checked
                   - (seq_t)6 * (seq_t)last_checked + (seq_t)1;
        seq_t t2 = (seq_t)16 * (seq_t)last_checked * (seq_t)last_checked
                   - (seq_t)10 * (seq_t)last_checked - (seq_t)1;

        /* Include n if either form is prime. */
        if (isPrimeGeneric(t1) || isPrimeGeneric(t2)) {
            /* Grow the cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure – terminate gracefully. */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
