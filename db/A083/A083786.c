/*
 * A083786  Composite numbers mod 10.
 * https://oeis.org/A083786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002808(n) mod 10 (nth composite number modulo 10)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Global state for memoisation and prime generation */
static seq_t *cache   = NULL;      /* cached values of a(1)…a(n) */
static size_t cache_len = 0;       /* number of cached values */

static idx_t *primes      = NULL;  /* list of known primes */
static idx_t primes_cnt   = 0;      /* number of primes stored */
static size_t primes_cap  = 0;      /* allocated capacity of primes array */

static idx_t next_candidate = 4;    /* next integer to test */
static size_t composites_done = 0;  /* number of composites already cached */

/* Ensure the prime list is initialised (contains 2 and 3). */
static void init_primes(void)
{
    if (primes == NULL) {
        primes_cap = 8;
        primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
        if (!primes) abort();
        primes_cnt = 0;
        /* add the two smallest primes */
        primes[primes_cnt++] = 2;
        primes[primes_cnt++] = 3;
    }
}

/* Append a new prime to the prime list, resizing if necessary. */
static void add_prime(idx_t p)
{
    if (primes_cnt >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 8;
        idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_cnt++] = p;
}

/* Simple deterministic primality test using the known primes list. */
static int is_prime(idx_t v)
{
    if (v < 2)                return 0;
    if (v == 2 || v == 3)    return 1;
    if (v % 2 == 0)           return 0;
    init_primes();            /* ensure primes list exists for subsequent tests */
    for (idx_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p > v / p) break; /* p*p > v -> no divisor */
        if (v % p == 0) return 0;
    }
    return 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined, but harmless */

    /* Expand cache if needed. */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = new_len;

        /* Continue generating composites until we have n values cached. */
        while (composites_done < n) {
            if (is_prime(next_candidate)) {
                add_prime(next_candidate);
            } else {
                cache[composites_done] = (seq_t)(next_candidate % 10);
                composites_done++;
            }
            next_candidate++;
        }
    }

    return cache[(size_t)(n - 1)];
}
