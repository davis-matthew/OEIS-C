/*
 * A096516  Solutions to A096509[x]=5; number of prime-powers [including
 *          primes] in the neighborhood of x with Ceiling[Log[x]] radius
 *          equals 5.
 * https://oeis.org/A096516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A096516 – The nth integer x such that the closed interval
   [x - ceil(log_e(x)), x + ceil(log_e(x))] contains exactly five
   prime‑powers (primes, squares, cubes, …).  The function a(n) returns
   a(1)=8, a(2)=10, ….
   The implementation precomputes all prime‑powers up to a fixed
   search limit (20 000 020) by a sieve of Eratosthenes, then scans
   the integers, counting prime‑powers in the required window by
   simple array look‑ups.  The window size grows only logarithmically,
   so the scan is essentially linear in the search range.  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Configuration  --------------------------------------------------- */
static const idx_t INITIAL_LIMIT = 20000020ULL;   /* must be >max needed value */

/* ------------------------------------------------------------------ */
/*  Static data  ----------------------------------------------------- */
static bool initialized = false;
static idx_t limit = INITIAL_LIMIT;

static uint8_t *is_pp = NULL;          /* is_pp[i] == 1 if i is a prime power */
static uint32_t *primes = NULL;        /* list of primes up to sqrt(limit) */
static size_t primes_cnt = 0;

static seq_t *terms = NULL;            /* discovered sequence terms */
static size_t terms_len  = 0;
static size_t terms_cap  = 0;
static idx_t search_x = 2;             /* next integer to examine */

/* thresholds[k] = floor(e^k)  (k = 0,1,2,…).  thresholds_count is minimal
   such that threshold[thresholds_count] > limit.  */
static uint64_t thresholds[64];
static unsigned thresholds_count = 0;

/* ------------------------------------------------------------------ */
/*  Utility functions  ----------------------------------------------- */

/* integer division: if (a > limit/p) then a*p would overflow */
static inline bool safe_mul(uint64_t a, uint64_t p, uint64_t *res) {
    if (a > limit / p) return false;
    *res = a * p;
    return true;
}

/* Compute ceil(log_e(x)) using the precomputed thresholds.  */
static uint8_t ceil_log_e(idx_t x) {
    unsigned lo = 0, hi = thresholds_count;      /* thresholds[0] = 1 */
    while (lo < hi) {
        unsigned mid = (lo + hi) >> 1;
        if (thresholds[mid] >= (uint64_t)x) hi = mid;
        else lo = mid + 1;
    }
    return (uint8_t)lo;
}

/* ------------------------------------------------------------------ */
/*  Precomputation  -------------------------------------------------- */

/* Fill thresholds array up to >limit.  */
static void init_thresholds(void) {
    double val = 1.0;
    thresholds_count = 0;
    while ((uint64_t)val <= limit) {
        thresholds[thresholds_count++] = (uint64_t)val;
        val *= 2.71828182845904523536;   /* e */
    }
    thresholds[thresholds_count++] = (uint64_t)val;   /* sentinel >limit */
}

/* Build primes up to sqrt(limit) and mark all prime powers up to limit. */
static void sieve_and_prime_powers(void) {
    /* Sieve of Eratosthenes (uint8_t array).  */
    uint8_t *is_prime = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!is_prime) { /* allocation failed – abort early */
        is_pp = NULL;
        return;
    }
    memset(is_prime, 1, (limit + 1) * sizeof(uint8_t));
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes to allocate array.  */
    primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++primes_cnt;

    primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) {
        free(is_prime);
        is_pp = NULL;
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = (uint32_t)i;

    /* Allocate is_pp array and mark all prime powers.  */
    is_pp = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!is_pp) {
        free(primes);
        primes = NULL;
        free(is_prime);
        return;
    }

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t power = p;
        while (power <= limit) {
            is_pp[(size_t)power] = 1;
            if (!safe_mul(power, p, &power)) break;
        }
    }

    free(is_prime);
}

/* Initialise all static data once.  */
static void ensure_initialised(void) {
    if (initialized) return;
    init_thresholds();
    sieve_and_prime_powers();
    terms_cap = 64;
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    terms_len = 0;
    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Main sequence generator  ---------------------------------------- */

seq_t a(idx_t n) {
    if (n < 1) return 0;           /* undefined for n=0 */
    ensure_initialised();
    if (!is_pp) return 0;          /* precomputation failed */

    while (terms_len < (size_t)n) {
        /* Examine current candidate x = search_x.  */
        idx_t x = search_x;
        uint8_t r = ceil_log_e(x);
        idx_t L = (x > (idx_t)r) ? (x - r) : 2;
        idx_t R = x + r;
        if (R > limit) R = limit;  /* should not happen with large limit */

        /* Count prime powers in [L,R].  */
        uint8_t count_pp = 0;
        for (idx_t i = L; i <= R; ++i) {
            if (is_pp[(size_t)i]) {
                if (++count_pp > 5) break;   /* early exit */
            }
        }
        if (count_pp == 5) {
            /* Store the term.  */
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                seq_t *new_terms = (seq_t *)realloc(terms,
                                                    terms_cap * sizeof(seq_t));
                if (!new_terms) return 0;   /* allocation failure */
                terms = new_terms;
            }
            terms[terms_len++] = (seq_t)x;
        }
        ++search_x;
    }
    return terms[(size_t)(n - 1)];
}
