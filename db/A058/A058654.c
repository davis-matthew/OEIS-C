/*
 * A058654  The sum of a prime and a nonzero square.
 * https://oeis.org/A058654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A058654: numbers that can be expressed as a prime plus a non‑zero square. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Prime sieve data */
static bool *prime_sieve = NULL;      /* true if index is prime */
static idx_t prime_limit   = 0;       /* last index of sieve */

/* Cache of computed sequence terms */
static seq_t *seq_cache   = NULL;     /* array of terms */
static idx_t  cache_len   = 0;        /* number of terms stored */
static idx_t  cache_cap   = 0;        /* capacity of seq_cache */
static idx_t  last_checked = 2;        /* last integer examined */

static void ensure_prime_limit(idx_t need)
{
    if (prime_limit >= need)
        return;

    idx_t new_lim = prime_limit > 0 ? prime_limit << 1 : 65536;
    while (new_lim < need) new_lim <<= 1;

    bool *new_sieve = (bool *)malloc((new_lim + 1) * sizeof(bool));
    if (!new_sieve)
        exit(1); /* allocation failed */

    for (idx_t i = 0; i <= new_lim; ++i)
        new_sieve[i] = true;
    new_sieve[0] = new_sieve[1] = false;

    for (uint64_t i = 2; i * i <= new_lim; ++i)
        if (new_sieve[i])
            for (uint64_t j = i * i; j <= new_lim; j += i)
                new_sieve[j] = false;

    free(prime_sieve);
    prime_sieve = new_sieve;
    prime_limit = new_lim;
}

/* Expand the cache if needed and store val. */
static void add_term(idx_t val)
{
    if (cache_len == cache_cap) {
        idx_t new_cap = cache_cap == 0 ? 32 : cache_cap << 1;
        seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_arr)
            exit(1); /* allocation failed */
        seq_cache = new_arr;
        cache_cap = new_cap;
    }
    seq_cache[cache_len++] = (seq_t)val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; return 0 for n==0 if caller uses 0‑based */
        return 0;

    /* Generate terms until we have n of them. */
    while (cache_len < n) {
        ++last_checked;
        if (last_checked <= 2)   /* skip numbers <= 2 */
            continue;

        /* Ensure we have primes at least up to last_checked. */
        ensure_prime_limit(last_checked);

        bool ok = false;
        for (idx_t s = 1; ; ++s) {
            uint64_t sq = (uint64_t)s * s;
            if (sq > (uint64_t)last_checked - 2)
                break;
            idx_t x = last_checked - (idx_t)sq;
            if (prime_sieve[x]) {      /* found prime + square representation */
                ok = true;
                break;
            }
        }

        if (ok)
            add_term(last_checked);
    }

    return seq_cache[n - 1];
}
