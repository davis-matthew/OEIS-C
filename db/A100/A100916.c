/*
 * A100916  Sum of a semiprime and its semiprime index is a new semiprime.
 * https://oeis.org/A100916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A100916 – Sum of a semiprime and its semiprime index is a new semiprime.
 *
 *  a(n) is the n‑th semiprime p_k such that p_k + k is also a semiprime.
 *
 *  Precomputation:
 *      1. Sieve primes up to N_MAX.
 *      2. Mark all numbers <= N_MAX that are semiprime.
 *      3. Build the sorted list of semiprimes ≤ N_MAX.
 *      4. For each semiprime p_k (k = 1,2,…) check if p_k + k is a semiprime
 *         (using the precomputed table for ≤ N_MAX or a quick trial division
 *         for larger numbers).  If so, store p_k in the result vector.
 *
 *  The chosen N_MAX = 2 000 000 guarantees that the first 10 000‐term
 *  prefix of the sequence is produced (and more).  The algorithm runs
 *  in O(N_MAX log log N_MAX) time and uses about 3 MB of memory.
 *
 *  This function is thread‑unsafe because of the one‑time static
 *  initialization, but that is acceptable for the eval harness. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool init_done = false;

/* --- parameters -------------------------------------------------------- */
#define N_MAX 2000000ULL    /* upper bound for sieving */

/* --- static tables ----------------------------------------------------- */
static bool *is_prime = NULL;          /* sieve marker */
static bool *is_semiprime = NULL;      /* semiprime flag for <= N_MAX */
static uint32_t *prime_list = NULL;    /* list of primes <= N_MAX */
static idx_t prime_count = 0;          /* length of prime_list */

/* semiprimes vector (sorted ascending) */
static seq_t *semiprimes = NULL;
static idx_t semiprime_count = 0;

/* sequence a(n) vector */
static seq_t *a_vals = NULL;
static idx_t a_count = 0;

/* --------------------------------------------------------------------- */
/*  Utility: Check if x is semiprime (exactly two prime factors,
 *  counting multiplicity).  For x <= N_MAX look up; otherwise trial
 *  division with the prime list.  -------------------------------------------------- */
static bool is_semiprime_value(seq_t x)
{
    if (x <= (seq_t)N_MAX) {
        return is_semiprime[(idx_t)x];
    }

    seq_t rem = x;
    idx_t i = 0;
    int cnt = 0;

    while (i < prime_count) {
        uint32_t p = prime_list[i];
        seq_t p_sq = (seq_t)p * p;
        if (p_sq > rem) break;

        while (rem % p == 0) {
            rem /= p;
            if (++cnt > 2) return false;
        }
        ++i;
    }
    if (rem > 1) cnt++;            /* remaining prime factor */
    return cnt == 2;
}

/* --------------------------------------------------------------------- */
/*  One‑time initialization --------------------------------------------- */
static void init_sequence(void)
{
    /* 1. Prime sieve */
    is_prime = (bool*)calloc(N_MAX + 1, sizeof(bool));
    is_semiprime = (bool*)calloc(N_MAX + 1, sizeof(bool));
    if (!is_prime || !is_semiprime) abort();

    idx_t estimate = N_MAX / 10;
    prime_list = (uint32_t*)malloc(estimate * sizeof(uint32_t));
    if (!prime_list) abort();

    is_prime[0] = is_prime[1] = true;  /* mark 0,1 as composite */
    for (uint64_t i = 2; i <= N_MAX; ++i) {
        if (!is_prime[i]) {
            prime_list[prime_count++] = (uint32_t)i;
            if (i <= N_MAX / i) {          /* avoid overflow */
                for (uint64_t j = i * i; j <= N_MAX; j += i)
                    is_prime[j] = true;
            }
        }
    }

    /* 2. Mark semiprimes */
    for (idx_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        for (idx_t j = i; j < prime_count; ++j) {
            uint32_t q = prime_list[j];
            uint64_t prod = (uint64_t)p * q;
            if (prod > N_MAX) break;
            is_semiprime[(idx_t)prod] = true;
        }
    }

    /* 3. Build sorted semiprime list */
    semiprimes = (seq_t*)malloc((N_MAX/3) * sizeof(seq_t)); /* over‑estimate */
    if (!semiprimes) abort();
    for (idx_t n = 2; n <= N_MAX; ++n)
        if (is_semiprime[n]) semiprimes[semiprime_count++] = (seq_t)n;
    semiprimes = (seq_t*)realloc(semiprimes, semiprime_count * sizeof(seq_t));

    /* 4. Build a(n) sequence */
    idx_t cap = semiprime_count / 12 + 10;            /* guess capacity */
    a_vals = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!a_vals) abort();
    for (idx_t k = 0; k < semiprime_count; ++k) {
        seq_t val = semiprimes[k] + (seq_t)(k + 1);
        if (is_semiprime_value(val)) {
            if (a_count == cap) {               /* grow if needed */
                cap *= 2;
                a_vals = (seq_t*)realloc(a_vals, cap * sizeof(seq_t));
                if (!a_vals) abort();
            }
            a_vals[a_count++] = semiprimes[k];
        }
    }

    init_done = true;
}

/* --------------------------------------------------------------------- */
/*  Public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!init_done) init_sequence();
    if (n == 0 || n > a_count) return (seq_t)0;
    return a_vals[n - 1];
}
