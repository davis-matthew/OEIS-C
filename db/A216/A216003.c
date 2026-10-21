/*
 * A216003  Primes that are the sum of 1501 consecutive primes.
 * https://oeis.org/A216003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime that is the sum of 1501 consecutive primes (OEIS
 *            A216003). The function searches consecutively through the list
 *            of primes, keeps a running sum of length 1501, and stores the
 *            sum whenever it is itself prime. The search is performed once on
 *            the first call to a(idx_t) and memoised. The...
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, build_primes_upto, isPrimeGeneric, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_realloc.h"

/* Number of consecutive primes to sum */
#define WINDOW 1501

/* Upper bound on the prime generation limit.  50 million produces
   enough terms (≈10 000) for all tests. */
#define PRIME_LIMIT 50000000ULL

/* Cached data ---------------------------------------------------- */
static uint64_t *g_primes   = NULL;  /* array of primes up to PRIME_LIMIT */
static size_t   g_prime_cnt = 0;     /* number of primes */

static seq_t *g_prefix = NULL;  /* prefix sums of primes: pref[i] = sum of first i primes */
static seq_t *g_terms  = NULL;  /* collected terms (prime sums) */
static size_t g_term_cnt = 0;   /* number of terms stored */
static size_t g_term_cap = 0;   /* allocated capacity for g_terms */

/* Grow the terms cache to hold at least new_cap elements */
static void grow_terms(size_t new_cap)
{
    g_terms = seq_realloc(g_terms, g_term_cap, new_cap);
    g_term_cap = new_cap;
}

/* Perform the single initialisation: build primes, prefixes, collect terms. */
static void init_once(void)
{
    if (g_primes != NULL)
        return;                   /* already initialised */

    /* 1. Build the list of primes up to PRIME_LIMIT */
    uint64_t *primes = NULL;
    size_t cnt = 0;
    build_primes_upto(PRIME_LIMIT, &primes, &cnt);
    g_primes   = primes;
    g_prime_cnt = cnt;

    /* 2. Allocate prefix sums (size = cnt + 1) */
    g_prefix = seq_realloc(NULL, 0, cnt + 1);
    g_prefix[0] = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        g_prefix[i + 1] = g_prefix[i] + (seq_t)g_primes[i];
    }

    /* 3. Allocate space for terms (initial capacity) */
    grow_terms(1024);

    /* 4. Scan all windows of length WINDOW and collect prime sums */
    for (size_t i = 0; i + WINDOW <= g_prime_cnt; ++i)
    {
        seq_t sum = g_prefix[i + WINDOW] - g_prefix[i];
        if (isPrimeGeneric(sum))
        {
            if (g_term_cnt == g_term_cap)
                grow_terms(g_term_cap * 2 + 1);
            g_terms[g_term_cnt++] = sum;
        }
    }
}

/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_once();                      /* ensure data is ready */

    /* a(1) is the first term; safe for n >= 1 */
    if ((size_t)n < 1 || (size_t)n > g_term_cnt)
        return 0;                     /* undefined or out of range */

    return g_terms[(size_t)n - 1];
}
