/*
 * A237348  Number of ordered ways to write n = k + m with k > 0 and m > 0
 *          such that prime(k) + 4 and prime(prime(m)) + 4 are both prime.
 * https://oeis.org/A237348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A237348  
   Formula: a(n) = # of ordered pairs (k,m) with k>0, m>0, k+m=n such that
             prime(k)+4 and prime(prime(m))+4 are both prime.  */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/* ---------- static data and helper functions ----------------------- */
/* ------------------------------------------------------------------ */
#define SIEVE_LIMIT 2000000ULL /* enough for n up to about 10000 */

static bool *sieve = NULL;
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

static bool *goodK = NULL;
static bool *goodM = NULL;
static seq_t *cache  = NULL;
static idx_t cache_size = 0;

static bool primes_built = false; /* indicates if sieve + primes are set up */

/* build a standard sieve up to SIEVE_LIMIT and collect primes */
static void build_primes(void)
{
    if (primes_built) return;

    sieve = (bool*)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    memset(sieve, true, SIEVE_LIMIT + 1);
    sieve[0] = sieve[1] = false;
    size_t r = (size_t)sqrt((double)SIEVE_LIMIT);
    for (size_t i = 2; i <= r; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = false;

    /* estimate number of primes for allocation */
    size_t est = (size_t)(SIEVE_LIMIT / log((double)SIEVE_LIMIT) * 1.2) + 1;
    primes = (uint32_t*)malloc(est * sizeof(uint32_t));
    primes_cnt = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (sieve[i]) {
            if (primes_cnt == est) {
                est *= 2;
                primes = (uint32_t*)realloc(primes, est * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;
        }

    primes_built = true;
}

/* ensure goodK and goodM arrays up to new_max_n and compute cache values */
static void ensure_cache_up_to(idx_t new_max_n)
{
    build_primes();

    /* grow goodK and goodM if necessary */
    if (goodK == NULL || goodM == NULL) {
        goodK = (bool*)malloc((new_max_n + 1) * sizeof(bool));
        goodM = (bool*)malloc((new_max_n + 1) * sizeof(bool));
    } else if (new_max_n + 1 > (size_t)cache_size) {
        goodK = (bool*)realloc(goodK, (new_max_n + 1) * sizeof(bool));
        goodM = (bool*)realloc(goodM, (new_max_n + 1) * sizeof(bool));
    }

    /* compute goodK and goodM for 1..new_max_n */
    for (idx_t k = 1; k <= new_max_n; ++k) {
        uint32_t pk = primes[k-1];
        goodK[k] = (pk + 4 <= SIEVE_LIMIT) && sieve[pk + 4];
    }
    for (idx_t m = 1; m <= new_max_n; ++m) {
        uint32_t pi_m = primes[m-1];
        uint32_t ppi_m = primes[pi_m - 1];
        goodM[m] = (ppi_m + 4 <= SIEVE_LIMIT) && sieve[ppi_m + 4];
    }

    /* grow cache if needed */
    if (cache == NULL) {
        cache = (seq_t*)malloc((new_max_n + 1) * sizeof(seq_t));
        cache[0] = cache[1] = 0;
    } else if (new_max_n + 1 > (size_t)cache_size) {
        cache = (seq_t*)realloc(cache, (new_max_n + 1) * sizeof(seq_t));
        /* keep existing values */
        for (idx_t i = cache_size; i <= new_max_n; ++i)
            cache[i] = 0;
    }

    /* compute cache values for n from cache_size to new_max_n */
    for (idx_t n = cache_size; n <= new_max_n; ++n) {
        seq_t cnt = 0;
        if (n >= 2) {
            for (idx_t k = 1; k <= n-1; ++k) {
                if (goodK[k] && goodM[n - k]) ++cnt;
            }
        }
        cache[n] = cnt;
    }

    cache_size = new_max_n + 1;
}

/* ------------------------------------------------------------------ */
/* ------------------- public function -------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if ((idx_t)cache_size <= n) {
        ensure_cache_up_to(n);
    }
    return cache[n];
}
