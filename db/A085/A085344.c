/*
 * A085344  Least number x such that number of primes between sigma(x) and x
 *          equals n.
 * https://oeis.org/A085344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min{x | π(σ(x)) – π(x) = n }, n ≥ 1
 * Range:     u8 n <= 35, u16 n <= 3227, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation note:   the function precomputes σ(x) and π(x) up
 *  to a sufficiently large x (here 2 000 000).  For each x it
 *  evaluates diff = π(σ(x)) – π(x) and remembers the smallest x that
 *  attains that diff.  The resulting lookup table is used for all
 *  subsequent calls to a().  The code is valid C11 and C++17. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Build tables the first time a() is called */
static const idx_t s_max_x = 2000000;                  /* maximum x to pre–compute */

static idx_t *sigma_arr  = NULL;   /* σ(x) for 1..s_max_x          */
static uint32_t *pi_arr   = NULL;   /* prime counts 0..prime_limit   */
static seq_t *minx_arr    = NULL;   /* minimal x for each diff       */
static size_t  minx_cap   = 0;       /* capacity of minx_arr          */

/* Allocate a buffer with a given capacity, copying old data */
static void grow_minx(idx_t new_cap) {
    seq_t *new_arr = (seq_t*)calloc((size_t)new_cap, sizeof(seq_t));
    for (size_t i = 0; i < minx_cap; ++i)
        new_arr[i] = minx_arr[i];
    free(minx_arr);
    minx_arr = new_arr;
    minx_cap = (size_t)new_cap;
}

/* Pre‑computation routine */
static void build_tables(void) {
    if (sigma_arr != NULL)
        return; /* already built */

    /* ---------- Compute σ(x) for 1..s_max_x ---------- */
    sigma_arr = (idx_t*)calloc((size_t)(s_max_x + 1), sizeof(idx_t));
    for (idx_t i = 1; i <= s_max_x; ++i) {
        for (idx_t j = i; j <= s_max_x; j += i)
            sigma_arr[j] += i;
    }

    /* ---------- Determine required prime limit ---------- */
    idx_t max_sigma = 0;
    for (idx_t x = 2; x <= s_max_x; ++x)
        if (sigma_arr[x] > max_sigma) max_sigma = sigma_arr[x];
    idx_t prime_limit = max_sigma + 100;   /* small safety buffer */

    /* ---------- Sieve of Eratosthenes up to prime_limit ---------- */
    unsigned char *is_prime = (unsigned char*)calloc((size_t)(prime_limit + 1), sizeof(unsigned char));
    for (idx_t i = 2; i <= prime_limit; ++i)
        is_prime[i] = 1;
    for (idx_t i = 2; (idx_t)(i * i) <= prime_limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= prime_limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* ---------- Prefix prime counts π(x) ---------- */
    pi_arr = (uint32_t*)calloc((size_t)(prime_limit + 1), sizeof(uint32_t));
    uint32_t count = 0;
    for (idx_t i = 2; i <= prime_limit; ++i) {
        if (is_prime[i]) ++count;
        pi_arr[i] = count;
    }
    free(is_prime);

    /* ---------- Compute minimal x for each diff ---------- */
    minx_cap = 1000;                         /* initial capacity */
    minx_arr = (seq_t*)calloc(minx_cap, sizeof(seq_t));
    for (idx_t x = 2; x <= s_max_x; ++x) {
        idx_t sig = sigma_arr[x];
        uint32_t diff = pi_arr[sig] - pi_arr[x];

        if (diff >= minx_cap)                   /* enlarge if needed */
            grow_minx(diff + 1000);

        if (minx_arr[diff] == 0)                 /* first occurrence */
            minx_arr[diff] = (seq_t)x;
    }
    free(sigma_arr);   /* no longer needed */
}

/* ------------------------------------------------------------------ */
/*  External function required by the judge                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    static bool initialized = false;
    if (!initialized) {
        build_tables();
        initialized = true;
    }
    if (n == 0 || (size_t)n >= minx_cap)   /* n outside pre‑computed range */
        return 0;
    return minx_arr[(size_t)n];
}
