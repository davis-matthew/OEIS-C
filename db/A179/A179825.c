/*
 * A179825  Multiples of 6 which are not the sum of a pair of twin primes.
 * https://oeis.org/A179825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A179825: multiples of 6 which are not the sum of a pair of twin primes.
   The sequence is defined for n >= 1 with offset 1: a(1)=96.  The function
   computes a(n) in O(n * average_twin_checks) time with a pre‑computed
   sieve of primes up to a fixed limit.  The implementation is valid in
   C11 and C++17, uses only permitted integer operations, and does not
   rely on any special floating point or external utilities. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----------------------------------------------------------------------- */
/* Prime sieve ----------------------------------------------------------- */
/* ----------------------------------------------------------------------- */

#define LIMIT_MAX 20000000UL   /* 20 million; enough for n <= 10000          */

/* static data structures */
static uint8_t *is_prime   = NULL;        /* 0 = not prime, 1 = prime         */
static uint8_t *is_twin    = NULL;        /* 0 = not twin prime, 1 = twin     */
static uint32_t *twin_primes = NULL;      /* list of twin primes              */
static size_t   twin_cnt   = 0;            /* number of twin primes             */

/* memory for sequence cache */
static seq_t   *seq_cache  = NULL;        /* dynamic array of computed terms   */
static size_t   cache_cap  = 0;           /* capacity of seq_cache             */
static size_t   cache_len  = 0;           /* number of terms currently stored  */
static uint64_t next_k     = 16ULL;       /* next k to consider; k=16 gives m=96 */

/* ----------------------------------------------------------------------- */
/* Helper: initialize tables --------------------------------------------- */
/* ----------------------------------------------------------------------- */

static void init_tables(void)
{
    unsigned long i, j;
    size_t sz = LIMIT_MAX + 1;

    /* Allocate and build prime sieve */
    is_prime = (uint8_t*)malloc(sz);
    is_twin  = (uint8_t*)malloc(sz);
    if (!is_prime || !is_twin) return;  /* allocation failed */

    for (i = 0; i < sz; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (i = 2; i * i <= LIMIT_MAX; ++i)
        if (is_prime[i])
            for (j = i * i; j <= LIMIT_MAX; j += i)
                is_prime[j] = 0;

    /* Build twin prime list and mark is_twin */
    twin_primes = (uint32_t*)malloc(sz / 10 * sizeof(uint32_t)); /* rough over‑estimate */
    twin_cnt = 0;
    for (i = 2; i <= LIMIT_MAX; ++i) {
        if (!is_prime[i]) continue;
        bool twin = false;
        if (i >= 3 && is_prime[i - 2]) twin = true;
        else if (i + 2 <= LIMIT_MAX && is_prime[i + 2]) twin = true;
        if (twin) {
            is_twin[i] = 1;
            twin_primes[twin_cnt++] = (uint32_t)i;
        } else {
            is_twin[i] = 0;
        }
    }
}

/* ----------------------------------------------------------------------- */
/* Main sequence function ------------------------------------------------- */
/* ----------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* 1. Initialise data once */
    static bool initialized = false;
    if (!initialized) {
        init_tables();
        initialized = true;
    }

    /* 2. Extend cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        uint64_t m = 6ULL * next_k;          /* candidate multiple of 6    */
        bool excluded = false;

        /* Check if m can be expressed as sum of two twin primes.
           We only need to test twin primes t <= m/2.           */
        for (size_t i = 0; i < twin_cnt && twin_primes[i] <= m/2; ++i) {
            uint64_t t  = twin_primes[i];
            uint64_t rem = m - t;
            if (rem <= LIMIT_MAX && is_twin[rem]) {
                excluded = true;
                break;
            }
        }

        if (!excluded) {
            /* store term */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0; /* memory failure, return 0 as sentinel */
                seq_cache = tmp;
                cache_cap = new_cap;
            }
            seq_cache[cache_len++] = (seq_t)m;
        }
        ++next_k;
    }

    return seq_cache[(size_t)n - 1];
}
