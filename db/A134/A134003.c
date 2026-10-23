/*
 * A134003  Positive integers n for which n^2+(n+5)^2 is prime.
 * https://oeis.org/A134003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 64, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth positive integer such that n^2+(n+5)^2 is prime */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

/* Returns the nth term of A134003.  The sequence is
   computed lazily and memoised; calls may come in any order. */
seq_t a(idx_t n)
{
    /* Static storage for memoisation */
    static seq_t *cache = NULL;      /* array of found n's */
    static size_t cache_len = 0;     /* number of terms stored      */
    static size_t cache_cap = 0;     /* capacity of the array       */
    static idx_t last_checked = 0;   /* last n that was examined    */

    /* Extend the cache until we have at least n terms */
    while (cache_len < n) {
        /* Next candidate */
        last_checked++;
        seq_t m = (seq_t)last_checked;

        /* Compute n^2 + (n+5)^2 = 2*n^2 + 10*n + 25 */
        seq_t val = 2 * m * m + 10 * m + 25;

        /* Test primality */
        if (is_prime_big(val)) {
            append_candidate(&cache, &cache_len, &cache_cap, m);
        }
    }

    /* Return the nth stored term (0‑based indexing) */
    return cache[n - 1];
}
