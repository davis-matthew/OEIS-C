/*
 * A078493  One-prime decades.
 * https://oeis.org/A078493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = starting number of the nth decade (multiple of 10) that *
 *            contains exactly one prime. * The code keeps a prime sieve
 *            growing as needed and scans decades * storing the ones with
 *            exactly one prime. Calls are handled in * arbitrary order
 *            through cached results
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* ----------  static data  ---------- */
static seq_t  *g_cache      = NULL;  /* cached decade starts          */
static size_t  g_cache_len  = 0;     /* number of cached elements     */
static size_t  g_cache_cap  = 0;     /* allocated capacity of cache   */

static unsigned char *g_prime_flag = NULL; /* 0 = composite, 1 = prime */
static idx_t g_sieve_limit = 0;             /* highest index sieved        */
static idx_t g_next_start  = 0;             /* next decade to examine      */

/* ----------  helper to grow the prime sieve  ----------
 *  Ensures that g_prime_flag is valid for indices up to N (inclusive).
 */
static void
ensure_sieve(idx_t N)
{
    if (N <= g_sieve_limit)
        return;   /* already enough */

    idx_t new_limit = (g_sieve_limit > 0) ? g_sieve_limit * 2 : N;
    if (new_limit < N)
        new_limit = N;

    /* grow the flag array */
    unsigned char *new_flag = (unsigned char *)realloc(g_prime_flag,
                                                       (size_t)(new_limit + 1)
                                                       * sizeof(unsigned char));
    if (!new_flag)
        abort();   /* memory exhausted */
    g_prime_flag = new_flag;

    /* initialise newly added entries to “prime” (1) */
    for (idx_t i = g_sieve_limit + 1; i <= new_limit; ++i)
        g_prime_flag[i] = 1;
    /* 0 and 1 are not prime */
    if (new_limit >= 0) g_prime_flag[0] = 0;
    if (new_limit >= 1) g_prime_flag[1] = 0;

    /* sieve the new range */
    idx_t limit_sqrt = (idx_t)sqrt((long double)new_limit);
    for (idx_t p = 2; p <= limit_sqrt; ++p) {
        if (!g_prime_flag[p])
            continue;
        /* start marking from the first multiple of p in the new range */
        idx_t start = p * p;
        if (start < g_sieve_limit + 1) {
            start = ((g_sieve_limit + 1 + p - 1) / p) * p;
        }
        for (idx_t j = start; j <= new_limit; j += p)
            g_prime_flag[j] = 0;
    }
    g_sieve_limit = new_limit;
}

/* ----------  public sequence routine  ----------
 *  Returns the nth term of A078493.
 */
extern seq_t a(idx_t n)
{
    /* Extend the cache until we have at least n+1 entries */
    while (g_cache_len <= n) {
        /* make sure we can inspect the next decade */
        ensure_sieve(g_next_start + 9);

        /* count primes in the decade [g_next_start, +9] */
        unsigned int cnt = 0;
        for (idx_t k = g_next_start; k < g_next_start + 10; ++k) {
            if (g_prime_flag[k])
                ++cnt;
        }
        if (cnt == 1) {
            /* store the decade start in the cache */
            if (g_cache_len == g_cache_cap) {
                size_t new_cap = g_cache_cap ? g_cache_cap * 2 : 16;
                seq_t *new_buf = (seq_t *)realloc(g_cache,
                                                  new_cap * sizeof(seq_t));
                if (!new_buf)
                    abort();   /* memory exhausted */
                g_cache = new_buf;
                g_cache_cap = new_cap;
            }
            g_cache[g_cache_len++] = (seq_t)g_next_start;
        }
        g_next_start += 10;   /* move to the next decade */
    }

    return g_cache[n];
}
