/*
 * A220269  a(n) is the smallest number, such that for all N >= a(n) there are
 *          at least n primes between 3*N and 4*N.
 * https://oeis.org/A220269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A220269 – a(n) is the smallest N such that for all M ≥ N there are
 * at least n primes in (3 M, 4 M].  The function pre‑computes prime
 * counts up to 4·MAX_LIMIT, builds the array
 *
 *   diff[N]  = π(4N) – π(3N)
 *
 * and then its suffix minima:
 *
 *   suff_min[N] = min{ diff[M] : M ≥ N }
 *
 * For a given n we binary‑search the smallest N with
 * suff_min[N] ≥ n.  The array sizes are chosen to cover all n
 * up to about 10 000 (MAX_LIMIT = 500 000).  The code works
 * for built‑in integer types as well as for the arbitrary‑precision
 * seq_t type supplied by the build.  It uses the utility
 * seq_alloc from the project to allocate the seq_t arrays. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"   /* provides seq_alloc(size_t) */

#define MAX_LIMIT 500000UL        /* upper bound on N for our precomputation */

static bool initialized = false;
static uint32_t *prime_pi = NULL; /* π(x) for 0 ≤ x ≤ 4 MAX_LIMIT */

static seq_t *diff   = NULL;      /* diff[N] = π(4N) – π(3N)   */
static seq_t *suff_min = NULL;    /* suffix minima of diff */

/* Simple sieve of Eratosthenes to fill prime_pi */
static void compute_prime_pi(void)
{
    const uint32_t MAX_PRIME = 4 * MAX_LIMIT;
    char *is_comp = (char *)malloc((size_t)(MAX_PRIME + 1));
    if (!is_comp) return;          /* out of memory → caller will see NULL */

    for (uint32_t i = 0; i <= MAX_PRIME; ++i) is_comp[i] = 0;
    uint32_t cnt = 0;
    prime_pi[0] = prime_pi[1] = 0;
    for (uint32_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_comp[i]) {
            ++cnt;
            if ((uint64_t)i * i <= MAX_PRIME) {
                for (uint32_t j = i * i; j <= MAX_PRIME; j += i)
                    is_comp[j] = 1;
            }
        }
        prime_pi[i] = cnt;
    }
    free(is_comp);
}

/* Precomputation of diff[] and its suffix minima */
static void precompute(void)
{
    const uint32_t MAX_PRIME = 4 * MAX_LIMIT;

    prime_pi = (uint32_t *)malloc((size_t)(MAX_PRIME + 1) * sizeof(uint32_t));
    diff     = seq_alloc((size_t)(MAX_LIMIT + 1));    /* indices 0..MAX_LIMIT */
    suff_min = seq_alloc((size_t)(MAX_LIMIT + 1));    /* indices 0..MAX_LIMIT */

    if (!prime_pi || !diff || !suff_min) return; /* allocation failure */

    compute_prime_pi();

    for (uint32_t i = 1; i <= MAX_LIMIT; ++i) {
        uint32_t p4 = prime_pi[4 * i];
        uint32_t p3 = prime_pi[3 * i];
        diff[i] = (seq_t)(p4 - p3);
    }

    /* build suffix minima (suff_min[MAX_LIMIT] = diff[MAX_LIMIT]) */
    suff_min[MAX_LIMIT] = diff[MAX_LIMIT];
    for (int64_t i = (int64_t)MAX_LIMIT - 1; i >= 1; --i) {
        suff_min[i] = (diff[(uint32_t)i] < suff_min[(uint32_t)i + 1])
                      ? diff[(uint32_t)i] : suff_min[(uint32_t)i + 1];
    }

    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!initialized) precompute();

    seq_t need = (seq_t)n;
    uint32_t l = 1, r = MAX_LIMIT;

    while (l < r) {
        uint32_t m = l + ((r - l) >> 1);
        if (suff_min[m] >= need)
            r = m;
        else
            l = m + 1;
    }
    return (seq_t)l;
}
