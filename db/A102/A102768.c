/*
 * A102768  Numbers k such that 23*k + 11 is prime.
 * https://oeis.org/A102768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that 23*k + 11 is prime
 * Range:     u8 n <= 4, u16 n <= 1899, u32 n <= 9862, u64 n <= 9862,
 *            u128 n <= 9862, big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime_seq.h"

/* Return the n-th term (1‑based) of A102768: numbers k for which 23*k+11 is prime. */
seq_t a(idx_t n)
{
    /* Static cache to avoid recomputing already found terms. */
    static seq_t *cache = NULL;    /* array of found k's, 0‑indexed */
    static idx_t  count = 0;       /* number of cached terms */
    static idx_t  cap   = 0;       /* capacity of cache array */

    /* Return immediately if the value is already cached. */
    if (n <= count && n > 0)
        return cache[n - 1];

    /* Ensure enough space in the cache. */
    if (cap < n) {
        idx_t new_cap = (cap == 0) ? 16 : cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* out of memory – unlikely in the test range */
        cache = tmp;
        cap = new_cap;
    }

    /* Search for additional terms until we have n terms. */
    idx_t k = (count == 0) ? 0 : (idx_t)cache[count - 1] + 1;
    while (count < n) {
        seq_t cand = ((seq_t)23 * k) + 11;
        if (is_prime_seq(cand)) {
            cache[count] = k;
            ++count;
        }
        ++k;
    }

    return cache[n - 1];
}
