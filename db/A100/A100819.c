/*
 * A100819  Composite numbers whose prime factors are twin primes.
 * https://oeis.org/A100819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   A100819 – composite numbers whose prime factors are twin primes
 * Range:     u8 none, u16 none, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses a humble‑number style algorithm with twin primes as the prime set. */
/* It caches generated values and handles any seq_t type (unsigned 8/16/32/64/128‑bit or arbitrary precision). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIEVE 2000000

static bool initialized = false;

/* Twin primes (unique prime list) */
static uint32_t *twin_primes = NULL;
static size_t twin_count = 0;

/* Humble numbers (including primes) */
static seq_t *humbles = NULL;
static size_t humbles_len = 0;
static size_t humbles_cap = 0;
static size_t *prime_idx = NULL;  /* index into humbles for each twin prime */

/* Result cache: composite values only */
static seq_t *results = NULL;
static size_t results_len = 0;
static size_t results_cap = 0;

/* ------------------------------------------------------------------ */
/* Utility: binary search to test whether a value is a twin prime.   */
/* ------------------------------------------------------------------ */
static bool is_twin_prime_seq(seq_t v) {
    size_t lo = 0, hi = twin_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t midv = (seq_t)twin_primes[mid];
        if (v == midv) return true;
        if (v < midv) hi = mid;
        else lo = mid + 1;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Generate twin primes up to MAX_SIEVE.                               */
/* ------------------------------------------------------------------ */
static void init_sieve(void) {
    bool *is_prime = (bool *)malloc((MAX_SIEVE + 1) * sizeof(bool));
    memset(is_prime, 1, (MAX_SIEVE + 1) * sizeof(bool));
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= MAX_SIEVE; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= MAX_SIEVE; j += i)
                is_prime[j] = 0;
        }
    }

    /* Gather twin primes (unique primes that are part of a twin pair) */
    uint32_t *tmp = (uint32_t *)malloc((MAX_SIEVE + 1) * sizeof(uint32_t));
    size_t cnt = 0;
    for (uint32_t p = 3; p <= MAX_SIEVE; ++p) {
        if (is_prime[p] &&
            ((p > 2 && is_prime[p - 2]) || (p + 2 <= MAX_SIEVE && is_prime[p + 2]))) {
            tmp[cnt++] = p;
        }
    }
    twin_primes = (uint32_t *)realloc(tmp, cnt * sizeof(uint32_t));
    twin_count = cnt;

    free(is_prime);
}

/* ------------------------------------------------------------------ */
/* Ensure the cache contains at least n composite terms.             */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n) {
    if (n <= results_len) return;                /* already available */

    /* Grow buffers as needed */
    if (results_cap < n) {
        results_cap = n + 64;
        results = (seq_t *)realloc(results, results_cap * sizeof(seq_t));
    }
    if (humbles_cap < (humbles_len + 128)) {
        humbles_cap = humbles_len + 128;
        humbles = (seq_t *)realloc(humbles, humbles_cap * sizeof(seq_t));
    }
    if (prime_idx == NULL) {                     /* allocate once */
        prime_idx = (size_t *)calloc(twin_count, sizeof(size_t));
    }

    /* Main generation loop */
    while (results_len < n) {
        seq_t best_candidate = 0;
        size_t best_pr = 0;
        bool best_set = false;

        /* Find the smallest next candidate */
        for (size_t j = 0; j < twin_count; ++j) {
            seq_t cand;
            if (sizeof(seq_t) == 8) {    /* efficient overflow check for uint64_t */
                uint64_t a = (uint64_t)humbles[prime_idx[j]];
                unsigned __int128 prod = (unsigned __int128)a * twin_primes[j];
                if (prod > UINT64_MAX) continue;   /* cannot represent */
                cand = (seq_t)prod;
            } else {
                cand = humbles[prime_idx[j]] * (seq_t)twin_primes[j];
            }
            if (!best_set || cand < best_candidate) {
                best_candidate = cand;
                best_set = true;
                best_pr = j;
            }
        }

        /* Unlikely, but guard against absence of candidates */
        if (!best_set) break;

        /* Append to humble list */
        humbles[humbles_len++] = best_candidate;

        /* Increment indices for all primes that produced this candidate */
        for (size_t j = 0; j < twin_count; ++j) {
            seq_t cand;
            if (sizeof(seq_t) == 8) {
                uint64_t a = (uint64_t)humbles[prime_idx[j]];
                unsigned __int128 prod = (unsigned __int128)a * twin_primes[j];
                if (prod > UINT64_MAX) continue;
                cand = (seq_t)prod;
            } else {
                cand = humbles[prime_idx[j]] * (seq_t)twin_primes[j];
            }
            if (cand == best_candidate) prime_idx[j]++;
        }

        /* Store in results if it is not a twin prime (i.e., composite) */
        if (!is_twin_prime_seq(best_candidate)) {
            results[results_len++] = best_candidate;
        }
    }
}

/* ------------------------------------------------------------------ */
/* One‑time initialization helper.                                    */
/* ------------------------------------------------------------------ */
static void init_once(void) {
    if (initialized) return;
    init_sieve();

    humbles_cap = 64;
    humbles = (seq_t *)malloc(humbles_cap * sizeof(seq_t));
    humbles[0] = 1;
    humbles_len = 1;

    results_cap = 64;
    results = (seq_t *)malloc(results_cap * sizeof(seq_t));

    /* prime_idx will be lazily allocated in ensure_cache */
    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Public function                                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    init_once();
    if (n == 0) return 0;                /* offset is 1 – undefined for n=0 */
    ensure_cache(n);
    return results[n - 1];
}
