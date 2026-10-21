/*
 * A202104  Numbers k such that 90*k + 41 is prime.
 * https://oeis.org/A202104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that 90*k + 41 is prime.
   The function builds an Eratosthenes sieve to test primality
   of numbers of the form 90*k+41 and caches the corresponding k
   values.  It works for any n ≥ 1, in arbitrary order of calls. */

#include <stdlib.h>
#include <stdbool.h>

static bool *sieve = NULL;          /* primality sieve */
static const idx_t SIEVE_LIMIT = 20000000ULL; /* fixed limit (≈2×10⁷) */

/* Cache of found k values */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static idx_t next_k = 0;

/* Initialise the sieve once */
static void init_sieve(void)
{
    if (sieve != NULL) return; /* already initialised */

    sieve = (bool *)calloc((size_t)(SIEVE_LIMIT + 1), 1);
    if (sieve == NULL) abort(); /* allocation failed */

    sieve[0] = true;
    sieve[1] = true;
    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = true;
        }
    }
}

/* Grow cache capacity if needed (doubling strategy) */
static void ensure_cache_capacity(void)
{
    if (cache_len < cache_cap) return;

    idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (new_cache == NULL) abort(); /* allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term (1‑based) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1; safety for n<1 */

    init_sieve();

    while (cache_len < n) {
        /* Candidate number of the form 90*k + 41 */
        uint64_t p = 90ULL * (uint64_t)next_k + 41ULL;

        if (p > SIEVE_LIMIT) abort(); /* should not happen for n≤≈10000 */

        bool is_prime = !sieve[p];
        if (is_prime) {
            ensure_cache_capacity();
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
