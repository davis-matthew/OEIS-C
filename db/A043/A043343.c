/*
 * A043343  Numbers m such that there is no k > m such that phi(k) = phi(m),
 *          where phi is Euler's totient function.
 * https://oeis.org/A043343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043343: numbers m for which no k > m satisfies φ(k)=φ(m).
   Algorithm: 1) Sieve totients up to a limit. 2) For each totient value
   record the largest m that attains it. 3) Collect these maxima, sort
   them, and answer queries from the sorted list. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Helper comparison for qsort on seq_t values. */
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

static idx_t *phi = NULL;          /* φ values, indexed by n */
static idx_t *max_phi = NULL;      /* max n achieving each φ value */
static seq_t *cache = NULL;        /* sorted list of maxima */
static size_t cache_size = 0;
static bool initialized = false;
static idx_t current_limit = 0;

static void build_up_to(idx_t limit)
{
    /* Free previously allocated arrays */
    free(phi);
    free(max_phi);
    free(cache);

    /* Allocate space for φ and the maximum tracker */
    phi = (idx_t *)malloc(sizeof(idx_t) * (limit + 1));
    max_phi = (idx_t *)malloc(sizeof(idx_t) * (limit + 1));
    if (!phi || !max_phi)
        return;   /* allocation failure – unlikely in test harness */

    /* Initialise φ(n)=n */
    for (idx_t i = 0; i <= limit; ++i)
        phi[i] = i;

    /* Euler sieve for φ */
    for (idx_t i = 2; i <= limit; ++i) {
        if (phi[i] == i) {             /* i is prime */
            for (idx_t j = i; j <= limit; j += i)
                phi[j] -= phi[j] / i;
        }
    }

    /* Initialise maximum array */
    for (idx_t i = 0; i <= limit; ++i)
        max_phi[i] = 0;

    /* Record maximum n for each totient value ≤ limit */
    for (idx_t n = 2; n <= limit; ++n) {
        idx_t v = phi[n];
        if (v != 0)
            max_phi[v] = n;            /* last (hence largest) n seen */
    }

    /* Count how many distinct totients have a maximum */
    size_t cnt = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (max_phi[i] != 0)
            ++cnt;

    /* Allocate and fill the cache with those maxima */
    cache = (seq_t *)malloc(sizeof(seq_t) * cnt);
    if (!cache)
        return;   /* allocation failure */

    size_t pos = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (max_phi[i] != 0)
            cache[pos++] = (seq_t)max_phi[i];

    /* Sort cache to provide ascending sequence */
    qsort(cache, cnt, sizeof(seq_t), cmp_seq);

    /* Update global state */
    cache_size = cnt;
    current_limit = limit;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0)                      /* offset starts at 1 */
        return (seq_t)0;

    /* Ensure cache large enough to answer query */
    if (!initialized || n > cache_size) {
        idx_t limit = initialized ? current_limit : 2000000U;
        while (n > cache_size) {
            build_up_to(limit);
            if (n > cache_size)
                limit *= 2;          /* grow and redo if needed */
        }
    }

    return cache[(size_t)(n - 1)];   /* zero‑based index */
}
