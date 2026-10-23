/*
 * A184876  Numbers m such that prime(m) is of the form
 *          k+floor(kr/t)+floor(ks/t), where r=log(2), s=log(3), t=log(5).
 * https://oeis.org/A184876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A184876
 *
 * a(n) = the n‑th integer m for which the n‑th prime number,
 *        prime(m), satisfies
 *        prime(m) = k + ⌊k·log(2)/log(5)⌋ + ⌊k·log(3)/log(5)⌋
 *        for some integer k ≥ 1.
 *
 * The function below builds the sequence by enumerating k,
 * computing the corresponding prime candidate, testing it with
 * a pre‑computed sieve, and storing the indices of the primes that
 * satisfy the condition.  Results are cached so that any order of
 * calls to a(idx_t) is handled efficiently.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Constants for the logarithm ratios (double precision).           */
static const double LOG2_RATIO = 0.69314718055994530941723212 /
                                 1.60943791243410037460075933;  /* ≈0.4306766 */
static const double LOG3_RATIO = 1.09861228866810969139524523 /
                                 1.60943791243410037460075933;  /* ≈0.6831471 */

/* ------------------------------------------------------------------ */
/* Primitive sieve upper bound.  Sufficient for n up to about 10 000.  */
static const uint32_t SIEVE_LIMIT = 2000000U;

/* ------------------------------------------------------------------ */
/* Static storage for the sieve and prime index lookup.               */
static bool            *is_prime     = NULL;
static uint32_t        *prime_index  = NULL;   /* 0 if composite, else 1‑based index */
static uint32_t        prime_count   = 0;       /* total number of primes */

/* ------------------------------------------------------------------ */
/* Build the sieve and prime index lookup once.                       */
static void init_prime_sieve(void)
{
    if (is_prime != NULL) return;  /* already initialized */

    /* Allocate memory */
    is_prime    = (bool*)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    prime_index = (uint32_t*)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));

    /* Initialise all entries as prime, then mark 0 and 1. */
    for (uint32_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    /* Eratosthenes sieve. */
    for (uint32_t p = 2; (uint64_t)p * p <= SIEVE_LIMIT; ++p) {
        if (is_prime[p]) {
            for (uint32_t m = p * p; m <= SIEVE_LIMIT; m += p)
                is_prime[m] = false;
        }
    }

    /* Build prime_index array and count primes. */
    prime_count = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            ++prime_count;
            prime_index[i] = prime_count;  /* 1‑based index */
        } else {
            prime_index[i] = 0;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Cached sequence terms and helper state.                            */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t next_k   = 1;

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
seq_t a(idx_t n)
{
    /* Initialise sieve and index data once. */
    if (is_prime == NULL) init_prime_sieve();

    /* Expand cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        uint64_t k = next_k;

        /* Compute floor(k*r/t) + floor(k*s/t) using double precision. */
        uint64_t f1 = (uint64_t)(k * LOG2_RATIO + 1e-12);
        uint64_t f2 = (uint64_t)(k * LOG3_RATIO + 1e-12);
        uint64_t p  = k + f1 + f2;   /* candidate prime value */

        if (p <= SIEVE_LIMIT && is_prime[(size_t)p]) {
            /* m = prime index of p */
            uint32_t m = prime_index[(size_t)p];
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap << 1 : 128;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)m;
        }

        ++next_k;
    }

    return cache[n - 1];
}
