/*
 * A099191  Smallest prime p such that p+n is twice its reversal, or 0 if
 *          impossible.
 * https://oeis.org/A099191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest prime p such that p+n = 2*rev(p); otherwise
 *            0
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/rev.h"

static const idx_t PRIME_LIMIT = 200000;          /* bound for prime sieve */

/* Prime storage */
static uint64_t *g_primes = NULL;
static size_t    g_prime_cnt = 0;

/* Prime sieve – generates all primes ≤ PRIME_LIMIT once */
static void generate_primes(void)
{
    if (g_primes != NULL)
        return; /* already generated */

    /* use a bit‑packedboolean array for simplicity; one byte per number */
    char *comp = (char*)calloc((size_t)PRIME_LIMIT + 1, (size_t)1);
    if (!comp)
        abort();

    for (idx_t i = 2; i * i <= (idx_t)PRIME_LIMIT; ++i)
        if (!comp[i])
            for (idx_t j = i * i; j <= (idx_t)PRIME_LIMIT; j += i)
                comp[j] = 1;

    /* count primes */
    g_prime_cnt = 0;
    for (idx_t i = 2; i <= (idx_t)PRIME_LIMIT; ++i)
        if (!comp[i])
            ++g_prime_cnt;

    /* allocate array for primes */
    g_primes = (uint64_t*)malloc(g_prime_cnt * sizeof(uint64_t));
    if (!g_primes)
        abort();

    /* fill array */
    size_t pos = 0;
    for (idx_t i = 2; i <= (idx_t)PRIME_LIMIT; ++i)
        if (!comp[i])
            g_primes[pos++] = (uint64_t)i;

    free(comp);
}

/* Cache for sequence values (1‑based) */
static seq_t *g_cache = NULL;
static size_t g_cache_cap = 0; /* allocated size (≥ needed) */
static idx_t  g_max_n_filled = 0; /* highest n that has been cached */

/* Grow cache so that it can hold index n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed <= g_cache_cap)
        return;

    size_t new_cap = g_cache_cap ? g_cache_cap << 1 : 256;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(g_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();

    /* initialise new slots to 0 */
    for (size_t i = g_cache_cap; i < new_cap; ++i)
        tmp[i] = (seq_t)0;

    g_cache = tmp;
    g_cache_cap = new_cap;
}

/* Fill cache entries for all indices up to n (inclusive) */
static void fill_cache_upto(idx_t n)
{
    if (n <= g_max_n_filled)
        return; /* already computed */

    generate_primes(); /* ensure primes exist */

    ensure_cache_size(n);

    seq_t two = (seq_t)2;

    for (size_t k = 0; k < g_prime_cnt; ++k)
    {
        uint64_t p = g_primes[k];
        idx_t rev_p = rev(p);
        seq_t rev_seq = (seq_t)rev_p;
        seq_t n_seq = two * rev_seq - (seq_t)p;

        if (n_seq == (seq_t)0)
            continue; /* would produce index 0 or negative ignore */

        if (n_seq > (seq_t)n)
            continue; /* index beyond current request */

        idx_t idx = (idx_t)n_seq; /* safe because n_seq ≤ n */

        if (idx >= 1 && idx <= n && g_cache[idx] == (seq_t)0)
            g_cache[idx] = (seq_t)p;
    }

    g_max_n_filled = n;
}

/* External function required by the BuildKit */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 0, return 0 */

    fill_cache_upto(n);
    return g_cache[n];
}
