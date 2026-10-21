/*
 * A237719  Numbers n such that k(n) = (n(n+1)/2 mod n) = (antisigma(n) mod n)
 *          + (sigma(n) mod n).
 * https://oeis.org/A237719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237719
   a(n) is the n‑th positive integer m for which
   σ(m) mod m <= m/2 if m is even, and
   σ(m) mod m == 0      if m is odd.
   The function builds σ(m) on demand and caches the
   sequence values. */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
static seq_t *sigma = NULL;          /* sum of divisors array, index 1..limit */
static idx_t sigma_limit = 0;        /* current maximum index covered by sigma */

static seq_t *cache = NULL;          /* sequence values a(1)..a(cache_size) */
static size_t cache_size = 0;        /* number of values stored */
static size_t cache_cap   = 0;       /* allocated capacity of cache */

static idx_t next_candidate = 1;     /* next integer to test for membership */

/* Build (or rebuild) sigma up to 'limit' (inclusive). */
static void build_sigma(idx_t limit)
{
    /* allocate new array of seq_t */
    seq_t *new_sigma;
#if defined(__cplusplus)
    new_sigma = new seq_t[(size_t)limit + 1];
#else
    new_sigma = (seq_t *)malloc(sizeof(seq_t) * ((size_t)limit + 1));
    if (!new_sigma) abort();
#endif
    /* initialize to zero */
    for (size_t i = 0; i <= (size_t)limit; ++i)
        new_sigma[i] = 0;

    /* divisor sieve: add i to all multiples of i */
    for (size_t i = 1; i <= (size_t)limit; ++i) {
        seq_t add = (seq_t)i;
        for (size_t j = i; j <= (size_t)limit; j += i)
            new_sigma[j] += add;
    }

    /* free old array */
#if defined(__cplusplus)
    delete[] sigma;
#else
    free(sigma);
#endif
    sigma = new_sigma;
    sigma_limit = limit;
}

/* Ensure that cache contains at least 'n' terms. */
static void ensure_cache(idx_t n)
{
    while ((idx_t)cache_size < n) {
        /* make sure sigma covers next_candidate */
        if ((idx_t)next_candidate > sigma_limit) {
            idx_t new_limit = sigma_limit ? sigma_limit * 2 : 10;
            /* grow until we cover next_candidate */
            while (new_limit < (idx_t)next_candidate + 1)
                new_limit *= 2;
            build_sigma(new_limit);
        }

        /* test membership for next_candidate */
        seq_t sigma_val = sigma[(size_t)next_candidate];
        seq_t mod_val   = sigma_val % (seq_t)next_candidate;
        seq_t half      = ((next_candidate & 1ULL) == 0) ?
                           (seq_t)(next_candidate / 2) : (seq_t)0;
        int in_seq = 0;

        if ((next_candidate & 1ULL) == 0) {           /* even */
            if (mod_val <= half) in_seq = 1;
        } else {                                      /* odd */
            if (mod_val == 0) in_seq = 1;
        }

        if (in_seq) {
            /* grow cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
#if defined(__cplusplus)
                seq_t *tmp = new seq_t[new_cap];
#else
                seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * new_cap);
                if (!tmp) abort();
#endif
                for (size_t i = 0; i < cache_size; ++i)
                    tmp[i] = cache[i];
#if defined(__cplusplus)
                delete[] cache;
#else
                free(cache);
#endif
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }
}

/* Return the nth term of A237719 (1‑based index). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
