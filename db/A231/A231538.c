/*
 * A231538  Number of n X 2 0..1 arrays with no element less than a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors.
 * https://oeis.org/A231538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 5*a(n-3) - 8*a(n-4) + 3*a(n-5) -
 *            2*a(n-6) + a(n-7) - a(n-8) for n >= 9, with initial values
 *            a(1)=2, a(2)=6, a(3)=20, a(4)=57, a(5)=164, a(6)=485, a(7)=1424,
 *            a(8)=4169
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for NULL, realloc */

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_computed = 0;
static const seq_t init_vals[8] = {2,6,20,57,164,485,1424,4169};

static void ensure_capacity(idx_t needed)
{
    if (cache_cap < (size_t)needed) {
        size_t new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1); /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used, offset starts at 1 */

    if (n <= 8) return init_vals[n-1];

    if (max_computed == 0) {
        /* allocate storage for first 8 values */
        ensure_capacity(9); /* indices 0..8 */
        for (idx_t i = 1; i <= 8; ++i) cache[i] = init_vals[i-1];
        max_computed = 8;
    }

    if (n > max_computed) {
        ensure_capacity(n+1); /* need indices up to n */
        for (idx_t i = max_computed+1; i <= n; ++i) {
            seq_t val = 4*cache[i-1];
            val -= 4*cache[i-2];
            val += 5*cache[i-3];
            val -= 8*cache[i-4];
            val += 3*cache[i-5];
            val -= 2*cache[i-6];
            val += cache[i-7];
            val -= cache[i-8];
            cache[i] = val;
        }
        max_computed = n;
    }

    return cache[n];
}
