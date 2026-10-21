/*
 * A058358  Coefficients in the series (1 + x + 4x^4 + 6x^6 + 8x^8 + 9x^9 +
 *          10x^10 + 12x^12 + 14x^14 + ... )/(1 - 2x^2 - 3x^3 - 5x^5 - 7x^7 -
 *          11x^11 - 13x^13 - ... ).
 * https://oeis.org/A058358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1 Recurrence: a(n) = b(n) + Σ_{p prime, p≤n} p
 *            · a(n-p), where b(n) = 1 (n = 0 or 1), 0 if n is prime ≥ 2, and
 *            b(n) = n if n is composite
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --- Globals for memoisation and prime generation ------------------------------------- */

static seq_t *a_cache = NULL;          /* cached values of a(n) */
static size_t a_cache_cap = 0;         /* current capacity (indices 0..cap-1) */
static idx_t a_max_computed = 0;       /* largest index that has been computed */

static bool *prime_sieve = NULL;       /* is_prime[k] */
static uint64_t *prime_list = NULL;    /* list of primes up to prime_limit */
static size_t prime_count = 0;         /* number of primes stored */
static idx_t prime_limit = 0;          /* maximum n for which primes are known */

/* --- Helper: build sieve and prime list up to 'limit' --------------------------------- */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* free old structures */
    free(prime_sieve);
    free(prime_list);

    /* allocate sieve */
    prime_sieve = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!prime_sieve) abort();
    for (idx_t i = 0; i <= limit; ++i)
        prime_sieve[i] = true;
    if (limit >= 0) prime_sieve[0] = false;
    if (limit >= 1) prime_sieve[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (prime_sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_sieve[j] = false;
        }
    }

    /* count primes */
    size_t np = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_sieve[i]) ++np;

    /* store prime list */
    prime_list = (uint64_t*)malloc(np * sizeof(uint64_t));
    if (!prime_list) abort();
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_sieve[i]) prime_list[idx++] = (uint64_t)i;

    prime_count = np;
    prime_limit = limit;
}

/* --- Helper: enlarge cache to hold indices up to 'n' ----------------------------------- */
static void
ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= a_cache_cap)
        return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    a_cache = new_cache;
    a_cache_cap = new_cap;
}

/* --- Main sequence function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (a_cache_cap == 0) {
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = (seq_t)1;          /* a(0) = 1 */
        a_cache_cap = 1;
        a_max_computed = 0;
    }

    /* Make sure we have necessary data structures */
    ensure_cache(n);
    ensure_primes(n);

    /* Compute values up to n if needed */
    if (n > a_max_computed) {
        idx_t start = a_max_computed + 1;
        for (idx_t i = start; i <= n; ++i) {
            seq_t bi;

            if (i == 0 || i == 1) {
                bi = (seq_t)1;
            } else if (prime_sieve[i]) {
                bi = (seq_t)0;
            } else {
                bi = (seq_t)i;   /* composite numbers */
            }

            seq_t sum = (seq_t)0;
            size_t k;
            for (k = 0; k < prime_count && prime_list[k] <= i; ++k) {
                idx_t p = (idx_t)prime_list[k];
                sum += (seq_t)p * a_cache[i - p];
            }

            a_cache[i] = bi + sum;
        }
        a_max_computed = n;
    }

    return a_cache[n];
}
