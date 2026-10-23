/*
 * A167896  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 5*4^(n-1) for 1 ≤ n ≤ 15, a(16) =
 *            3*∑_{j=1}^{15} a(16-j) – 6*a(0) + 1, a(n) = 3*∑_{j=1}^{15}
 *            a(n-j) – 6*a(n-16) for n ≥ 17 (OEIS A167896)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t cache_size = 0;   /* number of elements allocated in seq_cache */

/* Initial values for n = 0 .. 15 */
static const seq_t init_vals[16] = {
    /* a(0) .. a(15) */
    1ULL,
    5ULL,
    20ULL,
    80ULL,
    320ULL,
    1280ULL,
    5120ULL,
    20480ULL,
    81920ULL,
    327680ULL,
    1310720ULL,
    5242880ULL,
    20971520ULL,
    83886080ULL,
    335544320ULL,
    1342177280ULL
};

/* Ensure seq_cache contains at least n+1 entries.  The first 16 entries
   are initialized from init_vals; a(16) is computed on demand. */
static void
ensure_cache(idx_t n)
{
    if (cache_size >= n + 1)
        return;

    size_t new_size = cache_size ? cache_size * 2 : 32;
    if (new_size < (size_t)(n + 1))
        new_size = n + 1;

    seq_t *ptr = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
    if (!ptr)
        abort();  /* crash on OOM */

    if (seq_cache == NULL) {
        /* first allocation: fill known values 0..15 */
        for (idx_t i = 0; i < 16; ++i)
            ptr[i] = init_vals[i];
        /* a(16) will be computed lazily when needed */
    }

    seq_cache = ptr;
    cache_size = new_size;
}

/* Compute a(n) using the recurrence described above.  The result is
   cached so that subsequent calls are fast. */
seq_t
a(idx_t n)
{
    /* Ensure we have space for the requested index. */
    ensure_cache(n);

    /* Base case: n < 16 are already initialized. */
    if (n < 16)
        return seq_cache[n];

    /* Compute a(16) if it has not been computed yet. */
    if (n >= 16 && seq_cache[16] == 0) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= 15; ++j)
            sum += seq_cache[16 - j];          /* a(15) .. a(1) */
        seq_cache[16] = 3 * sum - 6 * seq_cache[0] + 1; /* +1 for n = 16 */
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = 17; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= 15; ++j)
            sum += seq_cache[i - j];
        seq_cache[i] = 3 * sum - 6 * seq_cache[i - 16];
    }

    return seq_cache[n];
}
