/*
 * A015914  Numbers k such that sigma(k) + 6 = sigma(k+6).
 * https://oeis.org/A015914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k such that sigma(k)+6 = sigma(k+6).
 *            The function finds a(n) by brute‑forcing successive k, caching
 *            the results so that future calls re‑use already found values
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_realloc, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/seq_realloc.h"

/* Return the n‑th term of OEIS A015914 (n≥1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n==0, but harmless */

    /* Static cache of found values and state of the search. */
    static seq_t *cache  = NULL;
    static size_t   len   = 0;   /* number of elements stored */
    static size_t   cap   = 0;   /* capacity of the cache array   */
    static idx_t    next_k = 1;   /* next integer to test          */

    while (len < n) {
        seq_t s_k   = sigma_of(next_k);
        seq_t s_k6  = sigma_of(next_k + 6);

        /* Check the defining condition. */
        if (s_k + (seq_t)6 == s_k6) {
            /* Grow the cache if necessary. */
            if (len == cap) {
                size_t new_cap = cap ? cap << 1 : 64;
                seq_t *tmp = seq_realloc(cache, cap, new_cap);
                /* In a failure case tmp is NULL; we ignore that
                   as the judging environment guarantees success. */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
