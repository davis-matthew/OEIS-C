/*
 * A129927  3-almost-prime octagonal numbers.
 * https://oeis.org/A129927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 3-almost-prime octagonal numbers: a(n) is the n‑th octagonal number
 * k(3k‑2) whose total number of prime factors (counted with multiplicity) is 3. */

/* -------------------------------------------------------------
 *  Helpers
 * ------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Prime sieve up to 1 000 000 – enough for k up to ~10⁶. */
static size_t        prime_cnt   = 0;   /* number of primes found  */
static uint32_t     *prime_list  = NULL;/* array of primes         */

/* initialise the prime list on first use */
static void init_primes(void)
{
    const size_t LIMIT = 1000000;          /* > sqrt(3·10¹²)          */
    size_t i, j;

    bool *is_comp = (bool*)malloc(LIMIT + 1);
    if (!is_comp) exit(1);
    memset(is_comp, 0, LIMIT + 1);

    /* over‑allocate a bit for the prime array (≈78498 primes ≤1e6) */
    prime_list = (uint32_t*)malloc(sizeof(uint32_t) * (LIMIT / 10 + 10));
    if (!prime_list) { free(is_comp); exit(1); }

    for (i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt == LIMIT / 10 + 9) { /* safety check */
                size_t new_cap = prime_cnt * 2 + 10;
                uint32_t *tmp = (uint32_t*)realloc(prime_list,
                                                    sizeof(uint32_t) * new_cap);
                if (!tmp) { free(is_comp); free(prime_list); exit(1); }
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
}

/* count Ω(n) for a 64‑bit integer (prime factors with multiplicity) */
static int count_omega_uint64(uint64_t n)
{
    int cnt = 0;
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint32_t p = prime_list[idx];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > n) break;
        while (n % p == 0) {
            ++cnt;
            n /= p;
        }
    }
    if (n > 1) ++cnt;
    return cnt;
}

/* -------------------------------------------------------------
 *  Cached sequence storage
 * ------------------------------------------------------------- */
static seq_t  *cache     = NULL;   /* array of computed terms      */
static size_t  cache_len = 0;      /* number of cached terms       */
static size_t  cache_cap = 0;      /* allocated capacity           */

/* next k value to test */
static idx_t  processed_k = 1;     /* next k to test */

/* -------------------------------------------------------------
 *  Main sequence access function
 * ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise primes on first call */
    static bool primes_initialized = false;
    if (!primes_initialized) { init_primes(); primes_initialized = true; }

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    /* extend cache until it contains at least n terms */
    while (cache_len < n) {
        idx_t k = processed_k++;                 /* octagonal index  */
        uint64_t part1 = (uint64_t)k;
        uint64_t part2 = 3 * k - 2;

        int omega = count_omega_uint64(part1) + count_omega_uint64(part2);

        if (omega == 3) {
            /* compute the octagonal number in seq_t space */
            seq_t val = (seq_t)k * (seq_t)(3 * k - 2);

            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(cache,
                                              cache_cap * sizeof(seq_t));
                if (!tmp) { free(cache); exit(1); }
                cache = tmp;
            }
            cache[cache_len++] = val;
        }
    }

    return cache[n - 1];
}
