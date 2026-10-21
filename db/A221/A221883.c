/*
 * A221883  Number of n X 2 arrays of occupancy after each element stays put
 *          or moves to some horizontal, vertical or antidiagonal neighbor,
 *          without consecutive moves in the same direction.
 * https://oeis.org/A221883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A221883
   Empirical recurrence:
   a(n) = 7*a(n-1) + 27*a(n-2) + 16*a(n-3) - 18*a(n-4) - 13*a(n-5)
          + 5*a(n-6) + 3*a(n-7) - a(n-8)
   Initial values:
     a(1)=3, a(2)=33, a(3)=326, a(4)=3223, a(5)=31832,
     a(6)=314426, a(7)=3105733, a(8)=30676867
*/

#include <stddef.h>

/* Choose a cache size comfortably covering n up to 10 000 */
#define MAX_CACHE 20000

/* Initial sequence values (1‑based) */
static const seq_t init_vals[8] = {
    (seq_t)3,
    (seq_t)33,
    (seq_t)326,
    (seq_t)3223,
    (seq_t)31832,
    (seq_t)314426,
    (seq_t)3105733,
    (seq_t)30676867
};

/* Helper that computes a(n) directly using the recurrence,
   without using the cache (for n > MAX_CACHE). */
static seq_t seq_direct_internal(idx_t n)
{
    /* For n <= 8 we can return the initial values directly. */
    if (n <= 8) return init_vals[n - 1];

    /* Start with the first 8 values in a temporary buffer. */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i) buf[i] = init_vals[i];

    for (idx_t k = 9; k <= n; ++k) {
        /* Compute next term using the recurrence.  Compute the
           positive part first to avoid unsigned underflow. */
        seq_t r = buf[7] * 7
                + buf[6] * 27
                + buf[5] * 16
                + buf[2] * 5
                + buf[1] * 3;
        r -= buf[4] * 18;
        r -= buf[3] * 13;
        r -= buf[0];

        /* Shift the buffer and store the new value. */
        for (idx_t j = 0; j < 7; ++j) buf[j] = buf[j + 1];
        buf[7] = r;
    }
    return buf[7];
}

/* Main function returning a(n).  It memoises values up to MAX_CACHE
   so that repeated calls are fast, even if the indices are given in
   arbitrary order. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* value for indices below the offset */

    /* If the requested index is beyond the cache size, compute it
       directly (no memoisation). */
    if (n > (idx_t)MAX_CACHE) return seq_direct_internal(n);

    /* Static cache of computed values.  cache[1] holds a(1). */
    static seq_t cache[MAX_CACHE + 1] = {0};
    static idx_t cache_len = 0;          /* largest n that has been computed */
    static int   cache_ready = 0;        /* initialised flag */

    /* Initialise the cache the first time it is needed. */
    if (!cache_ready) {
        for (idx_t i = 1; i <= 8; ++i) cache[i] = init_vals[i - 1];
        cache_len = 8;
        cache_ready = 1;
    }

    /* If the value is already cached, return it. */
    if (n <= cache_len) return cache[n];

    /* Compute new terms up to n and store them. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Use the same recurrence as in seq_direct_internal. */
        seq_t r = cache[i - 1] * 7
                + cache[i - 2] * 27
                + cache[i - 3] * 16
                + cache[i - 6] * 5
                + cache[i - 7] * 3;
        r -= cache[i - 4] * 18;
        r -= cache[i - 5] * 13;
        r -= cache[i - 8];
        cache[i] = r;
    }
    cache_len = n;
    return cache[n];
}
