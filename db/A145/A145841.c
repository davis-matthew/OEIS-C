/*
 * A145841  Number of 5-compositions of n.
 * https://oeis.org/A145841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Number of 5‑compositions of n Recurrence: for n ≥ 5, a(n)
 *            = 10*a(n-1) - 20*a(n-2) + 20*a(n-3) - 10*a(n-4) + 2*a(n-5) (n>5)
 *            a(5) = 10*a(4) - 20*a(3) + 20*a(2) - 10*a(1) + 2*a(0) - 1
 *            Initial values: a(0)=1, a(1)=5, a(2)=40, a(3)=310, a(4)=2395
 *            (The recurrence for n>5 has no constant term bec...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#if defined(__cplusplus)
#  define RECAST_SEQ_T(ptr) reinterpret_cast<seq_t*>(ptr)
#else
#  define RECAST_SEQ_T(ptr) ((seq_t*)(ptr))
#endif

/* Initial 5 terms */
static const seq_t init_vals[5] = {1, 5, 40, 310, 2395};

/* Dynamic cache: cache[i] holds a(i) for 0 ≤ i ≤ max_computed */
static seq_t *cache       = NULL;
static idx_t cache_cap    = 0;
static idx_t max_computed = 4;          /* largest index currently known */

/* Ensure that the cache can hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap) return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    if (new_cap <= n) new_cap = n + 1;

    seq_t *new_arr = RECAST_SEQ_T(realloc(cache, sizeof(seq_t) * new_cap));
    if (!new_arr) return;               /* allocation failed – leave old cache */

    /* If this is the first allocation, copy the initial values */
    if (cache == NULL) {
        for (idx_t i = 0; i < 5; ++i)
            new_arr[i] = init_vals[i];
    }

    cache      = new_arr;
    cache_cap  = new_cap;
}

/* Computes a(n) for any n ≥ 0 in any order of calls */
seq_t a(idx_t n)
{
    /* For n < 5 the result is known immediately */
    if (n < 5) return init_vals[n];

    /* Initialize the cache on first use */
    if (cache == NULL) {
        ensure_capacity(5);                      /* to hold indices 0..4 */
        if (cache == NULL) return 0;             /* fallback on allocation error */
    }

    /* Compute missing values up to n (if any) */
    if (n > max_computed) {
        ensure_capacity(n);
        if (cache == NULL) return 0;             /* allocation failure */

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* Apply the recurrence */
            seq_t v = cache[i-1] * 10;
            v -= cache[i-2] * 20;
            v += cache[i-3] * 20;
            v -= cache[i-4] * 10;
            v += cache[i-5] * 2;
            if (i == 5) v -= 1;                 /* constant term for i == 5 */
            cache[i] = v;
        }
        max_computed = n;
    }

    return cache[n];
}
