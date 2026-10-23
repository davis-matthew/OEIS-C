/*
 * A134420  Composite squarefree numbers of the form k^2 + 1.
 * https://oeis.org/A134420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 186, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A134420: composite squarefree numbers of the form k^2 + 1
   (offset 1). The function builds the sequence lazily, caching terms
   in a static array.  It generates a list of primes once using a
   simple sieve up to PRIMES_LIMIT, then checks each k^2+1 for
   compositeness and square‑freeness by trial division with that list. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- prime sieve (up to 200 000) ---------- */
#define PRIMES_LIMIT 200000

static bool primes_initialized = false;
static uint32_t primes[PRIMES_LIMIT + 1];
static size_t prime_count = 0;

/* initialise the prime table once */
static void init_primes(void)
{
    if (primes_initialized) return;
    bool *sieve = (bool *)malloc(PRIMES_LIMIT + 1);
    if (!sieve) abort();
    for (size_t i = 0; i <= PRIMES_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= PRIMES_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIMES_LIMIT; j += i)
                sieve[j] = false;
        }
    }
    for (size_t i = 2; i <= PRIMES_LIMIT; ++i)
        if (sieve[i]) primes[prime_count++] = (uint32_t)i;
    free(sieve);
    primes_initialized = true;
}

/* ---------- check k^2 + 1 is square‑free and composite ---------- */
static bool is_squarefree_composite(seq_t val)
{
    seq_t t = val;
    int distinct = 0;
    for (size_t i = 0; i < prime_count &&
         ((seq_t)primes[i] * (seq_t)primes[i] <= t); ++i) {
        uint32_t p = primes[i];
        seq_t rem = t % (seq_t)p;
        if (rem == 0) {
            ++distinct;
            t /= (seq_t)p;
            if (t % (seq_t)p == 0)  /* repeated prime factor -> not squarefree */
                return false;
        }
    }
    if (t > (seq_t)1) ++distinct;   /* leftover prime factor */
    return distinct >= 2;    /* composite & square‑free */
}

/* ---------- lazy cache of the sequence ---------- */
static seq_t  *cache  = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity */
static idx_t   last_k = 0;     /* last k processed */

/* expand cache capacity */
static void ensure_cache(void) {
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* generate terms until at least n terms are available */
static void ensure_n_terms(idx_t n)
{
    if (n <= (idx_t)cache_len) return;
    init_primes();
    while ((idx_t)cache_len < n) {
        ++last_k;
        if (last_k < 3) continue;            /* 1^2+1 = 2, 2^2+1 = 5 are prime */
        seq_t val = (seq_t)last_k * (seq_t)last_k + ((seq_t)1);
        if (is_squarefree_composite(val)) {
            ensure_cache();
            cache[cache_len++] = val;
        }
    }
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    /* the OEIS offset starts at n = 1 */
    if (n == 0) return ((seq_t)0);
    ensure_n_terms(n);
    return cache[n - 1];
}
