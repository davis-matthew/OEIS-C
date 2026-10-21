/*
 * A209780  Half the number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock
 *          having exactly one duplicate clockwise edge difference.
 * https://oeis.org/A209780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A209780
   Empirical recurrence:
   a(n) = 7*a(n-1) + 22*a(n-2) - 39*a(n-3) - 95*a(n-4) + 30*a(n-6)
   Initial values (n = 1..6):
   a(1)=64, a(2)=576, a(3)=5074, a(4)=44948, a(5)=397734, a(6)=3520628
   The sequence is indexed from 1; calls with n<1 are undefined. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache with 1‑based indexing */
    static seq_t *cache   = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity (>= cache_size) */
    static idx_t cache_size = 0;  /* highest index that has been computed */

    /* Helper to ensure capacity for at least `need` indices (1‑based). */
    if (cache_cap < n + 1) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < n + 1) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* First time: initialise the first six terms. */
    if (cache_size == 0) {
        /* Guarantee capacity for at least 6 entries. */
        if (cache_cap < 6 + 1) {
            idx_t new_cap = 6 + 1;
            cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
            cache_cap = new_cap;
        }
        cache[1] = (seq_t)64;
        cache[2] = (seq_t)576;
        cache[3] = (seq_t)5074;
        cache[4] = (seq_t)44948;
        cache[5] = (seq_t)397734;
        cache[6] = (seq_t)3520628;
        cache_size = 6;
    }

    /* Extend the cache up to n if necessary.  Recurrence requires
       values down to n-6, which is valid for n>6. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val = (seq_t)7 * cache[i-1];
        val += (seq_t)22 * cache[i-2];
        val -= (seq_t)39 * cache[i-3];
        val -= (seq_t)95 * cache[i-4];
        val += (seq_t)30 * cache[i-6];
        cache[i] = val;
    }
    if (cache_size < n) cache_size = n;

    return cache[n];
}
