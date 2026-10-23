/*
 * A037626  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,0.
 * https://oeis.org/A037626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 2; a(n) = 5*a(n-1) + d(n),
   where d(n) = 2 if n%3==1, 3 if n%3==2, 0 otherwise. */
#include <stddef.h>
#include "../utils/grow_seq_array.h" /* ensures capacity dynamically */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* allocated capacity */
    static size_t cache_used = 0; /* number of elements stored (a(1)..a(cache_used)) */

    /* Ensure the cache can hold at least n elements. */
    if (n > (idx_t)cache_cap) {
        /* Use utility that grows or allocates as needed. */
        grow_seq_array((seq_t **)&cache, &cache_cap, (size_t)n);
    }

    /* Compute missing terms up to n. */
    if (cache_used == 0) {
        /* Compute first term a(1) = 2. */
        cache[0] = (seq_t)2;
        cache_used = 1;
    }

    for (idx_t k = cache_used + 1; k <= n; ++k) {
        seq_t prev = cache[k - 2]; /* a(k-1) */
        idx_t mod = k % 3;
        seq_t digit = (mod == 1) ? (seq_t)2 :
                      (mod == 2) ? (seq_t)3 : (seq_t)0;
        cache[k - 1] = prev * (seq_t)5 + digit;
    }
    cache_used = n;

    return cache[n - 1];
}
