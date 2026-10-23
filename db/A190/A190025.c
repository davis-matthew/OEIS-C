/*
 * A190025  Number of (n+2) X 3 binary arrays with every 3 X 3 subblock
 *          commuting with each horizontal and vertical neighbor 3 X 3
 *          subblock.
 * https://oeis.org/A190025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) + a(n-6) -
 *            a(n-7) for n > 11 Initial values: a(1)=512, a(2)=112, a(3)=228,
 *            a(4)=415, a(5)=616, a(6)=880, a(7)=1205, a(8)=1508, a(9)=2008,
 *            a(10)=2793, a(11)=3776
 * Range:     u8 none, u16 n <= 20, u32 n <= 56, u64 n <= 128, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define MAX_CACHE 10000

static seq_t a_cache[MAX_CACHE + 1];
static bool a_cached = false;

/* Known values for indices 1..11 (index 0 unused). */
static const seq_t a_base[12] = {
    (seq_t)0,      /* unused */
    (seq_t)512,    /* a(1) */
    (seq_t)112,    /* a(2) */
    (seq_t)228,    /* a(3) */
    (seq_t)415,    /* a(4) */
    (seq_t)616,    /* a(5) */
    (seq_t)880,    /* a(6) */
    (seq_t)1205,   /* a(7) */
    (seq_t)1508,   /* a(8) */
    (seq_t)2008,   /* a(9) */
    (seq_t)2793,   /* a(10) */
    (seq_t)3776    /* a(11) */
};

/* One‑time initialisation of the cache up to MAX_CACHE. */
static void init_cache(void)
{
    /* Initialise the first 11 terms. */
    for (idx_t i = 1; i <= 11; ++i)
        a_cache[i] = a_base[i];

    /* Compute the rest using the 7‑term recurrence. */
    for (idx_t i = 12; i <= MAX_CACHE; ++i) {
        a_cache[i] =
            (seq_t)2 * a_cache[i-1] -
            a_cache[i-2] +
            a_cache[i-3] -
            (seq_t)2 * a_cache[i-4] +
            a_cache[i-5] +
            a_cache[i-6] -
            a_cache[i-7];
    }
    a_cached = true;
}

/* Public sequence routine. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0; /* undefined for n < 1 */

    /* Use the cached values for small n. */
    if (n <= (idx_t)MAX_CACHE) {
        if (!a_cached)
            init_cache();
        return a_cache[n];
    }

    /* For n > MAX_CACHE we compute on the fly using only the recurrence
       and the first 11 known terms.  A temporary array up to n is allocated
       and freed afterwards. */
    seq_t *dyn = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!dyn)                /* allocation failed */
        return (seq_t)0;

    dyn[0] = (seq_t)0;       /* unused */
    for (idx_t i = 1; i <= (idx_t)11 && i <= n; ++i)
        dyn[i] = a_base[i];

    for (idx_t i = 12; i <= n; ++i) {
        dyn[i] =
            (seq_t)2 * dyn[i-1] -
            dyn[i-2] +
            dyn[i-3] -
            (seq_t)2 * dyn[i-4] +
            dyn[i-5] +
            dyn[i-6] -
            dyn[i-7];
    }

    seq_t result = dyn[n];
    free(dyn);
    return result;
}
