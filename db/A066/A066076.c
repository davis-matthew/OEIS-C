/*
 * A066076  Primes p such that there is a unique solution to p = sigma(x) - 1.
 * https://oeis.org/A066076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3912, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes p such that the equation p = sigma(x) - 1 has a unique solution.
   The function precomputes sigma values up to a fixed bound and builds the sequence
   of such primes.  The bound is large enough to cover thousands of terms. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* The sequence function requested by the harness */
seq_t a(idx_t n);

/* ---------- internal helpers ---------- */

static bool init_done = false;
static idx_t *sigma_arr      = NULL;   /* sigma[i] = sum of divisors of i   */
static uint16_t *cnt         = NULL;   /* occurrence count of each sigma value */
static idx_t *first_occurrence = NULL; /* smallest n producing a sigma value */
static idx_t *prime_list      = NULL;  /* list of primes up to LIMIT */
static size_t prime_cnt       = 0;
static seq_t *seq_terms       = NULL;  /* resulting sequence terms */
static idx_t seq_cnt          = 0;

#define LIMIT 1000000          /* upper bound for precomputation  */
#define INITIAL_CAP 1024       /* initial capacity for sequence terms */

/* Precomputation performed once */
static void init(void) {
    if (init_done) return;
    init_done = true;

    /* ---- 1. Compute sigma values for 1 .. LIMIT ---- */
    sigma_arr = (idx_t *)calloc((LIMIT + 1), sizeof(idx_t));
    if (!sigma_arr) return; /* Allocation failed */

    for (idx_t i = 1; i <= LIMIT; ++i) {
        for (idx_t j = i; j <= LIMIT; j += i) {
            sigma_arr[j] += i;
        }
    }

    /* ---- 2. Determine maximal sigma value ---- */
    idx_t max_sigma = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (sigma_arr[i] > max_sigma) max_sigma = sigma_arr[i];
    }

    /* ---- 3. Allocate arrays for counting ---- */
    cnt = (uint16_t *)calloc((size_t)(max_sigma + 1), sizeof(uint16_t));
    first_occurrence = (idx_t *)calloc((size_t)(max_sigma + 1), sizeof(idx_t));
    if (!cnt || !first_occurrence) return; /* Allocation failed */

    /* ---- 4. Fill counts and first occurrences ---- */
    for (idx_t i = 1; i <= LIMIT; ++i) {
        idx_t val = sigma_arr[i];
        if (!cnt[val]) first_occurrence[val] = i;
        cnt[val]++;
    }

    /* ---- 5. Sieve primes up to LIMIT ---- */
    bool *is_prime = (bool *)calloc((LIMIT + 1), sizeof(bool));
    if (!is_prime) return;
    for (idx_t i = 2; i <= LIMIT; ++i) is_prime[i] = true;
    for (idx_t i = 2; (idx_t)(i * i) <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
        }
    }

    /* Estimate number of primes: less than LIMIT / log(LIMIT) */
    size_t est_primes = (size_t)(LIMIT / 10);
    prime_list = (idx_t *)malloc(est_primes * sizeof(idx_t));
    if (!prime_list) return;

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            if (prime_cnt == est_primes) {
                /* expand array if necessary */
                est_primes = est_primes * 2 + 16;
                prime_list = (idx_t *)realloc(prime_list, est_primes * sizeof(idx_t));
                if (!prime_list) return;
            }
            prime_list[prime_cnt++] = i;
        }
    }

    /* ---- 6. Build sequence terms (unique sigma primes) ---- */
    size_t cap = INITIAL_CAP;
    seq_terms = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!seq_terms) return;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = prime_list[pi];
        idx_t s = p + 1;
        if (s <= max_sigma && cnt[s] == 1 && first_occurrence[s] == p) {
            if (seq_cnt == cap) {
                cap = cap * 2 + 16;
                seq_terms = (seq_t *)realloc(seq_terms, cap * sizeof(seq_t));
                if (!seq_terms) return;
            }
            seq_terms[seq_cnt++] = (seq_t)p;
        }
    }

    /* Free temporary buffers that are no longer needed */
    free(sigma_arr);
    free(cnt);
    free(first_occurrence);
    free(is_prime);
    free(prime_list);
}

/* ---------- public interface ---------- */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* Undefined for index 0 */

    init();                        /* Ensure precomputation done */

    if (n - 1 >= seq_cnt) return (seq_t)0;  /* Out‑of‑range query */

    return seq_terms[n - 1];
}
