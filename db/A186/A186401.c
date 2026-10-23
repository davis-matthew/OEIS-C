/*
 * A186401  Numbers k such that there are 9 primes between 100*k and 100*k +
 *          99.
 * https://oeis.org/A186401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   k such that there are exactly 9 primes in [100*k,100*k+99]
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine generates terms on demand, caching them.  The sieve is
   built lazily once enough primes are needed to cover the requested
   centuries.  This implementation works for all builds (primitive
   integer types and arbitrary‑precision integer classes). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- fixed sieve size (enough for n up to about 10 000) ---------- */
#define PRIME_LIMIT 25000000ULL   /* covers k up to ~250 000 → 100*k+99 < 25 million */

static unsigned char *prime_flags = NULL;

/* build sieve once – cast to a C++ compatible type as required */
static void init_sieve(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    prime_flags = (unsigned char*)malloc(PRIME_LIMIT + 1);
    if (!prime_flags) return;  /* allocation failure is unlikely */

    memset(prime_flags, 1, PRIME_LIMIT + 1);
    prime_flags[0] = prime_flags[1] = 0;

    for (uint64_t p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (prime_flags[p]) {
            for (uint64_t x = p * p; x <= PRIME_LIMIT; x += p)
                prime_flags[x] = 0;
        }
    }
}

/* ---------- cache for sequence values ---------- */
#define CACHE_MAX 200000   /* should be > expected number of valid k for n=10000 */
static seq_t cache[CACHE_MAX];
static size_t cache_len = 0;          /* number of elements stored */
static uint64_t last_k = 0;           /* last century processed */
static bool cache_ready = false;      /* whether cache[0] is valid */

/* add a new value to the cache – assumes space is available */
static void add_cache(seq_t val)
{
    cache[cache_len++] = val;
}

/* ---------- public sequence routine ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n<1, safe return */

    /* Build cache up to required index */
    if (!cache_ready) {
        cache_len = 0;
        last_k = 0;
        cache_ready = true;
    }

    init_sieve();

    while (cache_len < (size_t)n) {
        uint64_t k = last_k + 1;
        uint64_t start = k * 100ULL;
        uint64_t end   = start + 99ULL;

        /* In case the interval exceeds the sieve – it should not happen
           for the problem limits. */
        if (end > PRIME_LIMIT) {
            /* If we ever hit this, enlarge the sieve (not needed here). */
            break;
        }

        uint64_t cnt = 0;
        for (uint64_t x = start; x <= end; ++x)
            if (prime_flags[x]) ++cnt;

        if (cnt == 9)
            add_cache((seq_t)k);

        last_k = k;
    }

    return cache[(size_t)n - 1];
}
