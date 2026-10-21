/*
 * A051616  Primes p for which A051614(p) is 3 but are not Sophie Germain
 *          primes.
 * https://oeis.org/A051616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A051616: primes p such that
 *  2p+1 is composite and the least prime divisor of
 *  2p·F(2p+1)+1 equals 3.
 *  F(x) denotes the least prime divisor of x.
 *
 *  Since 2p·F(2p+1)+1 is odd, its least prime divisor is 3
 *  iff it is divisible by 3.  This reduces to checking
 *  ((p mod 3) * (F(2p+1) mod 3)) ≡ 1 (mod 3).
 *
 *  The function below pre-sieves primes up to a modest limit
 *  (20 000 000).  It then scans the primes, skipping Sophie
 *  Germain primes (where 2p+1 is prime), computing the least
 *  prime divisor of 2p+1 by trial division with the sieve
 *  primes, and storing matching primes in a cache.  The cache
 *  supports arbitrary call order: the function keeps generating
 *  terms until the requested index is reached.  All arithmetic
 *  uses idx_t and seq_t as defined by the build; no built‑in
 *  overflows are involved for the range of n up to about 10 000.
 */

/* Standard headers for the helpers used below. */
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* -------------------------------------------------------------------- */
/*  SIF LET UTILITIES ------------------------------------------------- */
/* -------------------------------------------------------------------- */

/* Upper limit for the pre-sieve.  20 000 000 covers more than
 * enough primes to find 10 000 terms of this sequence. */
#define SIEVE_LIMIT     20000000U
/* Expected count of primes below SIEVE_LIMIT (≈ 1.27 million). */
#define MAX_PRIME_COUNT 1300000U

static uint8_t  is_prime_arr[SIEVE_LIMIT + 1];          /* 0 = composite, 1 = prime */
static idx_t   primes_arr[MAX_PRIME_COUNT];
static size_t  prime_count = 0;

/* Simple linear sieve of Eratosthenes. */
static void init_sieve(void)
{
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime_arr[i] = 1;
    is_prime_arr[0] = is_prime_arr[1] = 0;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime_arr[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime_arr[j] = 0;
        }
    }

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime_arr[i])
            primes_arr[prime_count++] = (idx_t)i;
}

/* Fast primality test for values within the sieve. */
static inline bool is_prime_small(idx_t n)
{
    return (n <= SIEVE_LIMIT) ? (is_prime_arr[n] != 0) : false;
}

/* Trial division primality test using the precomputed primes. */
static bool is_prime_trial(idx_t n)
{
    if (n < 2)            return false;
    if (n == 2)           return true;
    if ((n & 1) == 0)     return false;   /* even numbers >2 are composite */

    idx_t limit = (idx_t) sqrt((double)n);
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = primes_arr[i];
        if (p > limit) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Least prime divisor of n, using the precomputed primes. */
static idx_t least_prime_factor(idx_t n)
{
    if (n < 2) return n;            /* undefined but harmless */
    idx_t limit = (idx_t) sqrt((double)n);
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = primes_arr[i];
        if (p > limit) break;
        if (n % p == 0) return p;
    }
    return n;                       /* n is prime */
}

/* -------------------------------------------------------------------- */
/*  SEQUENCE HANDLING ------------------------------------------------- */
/* -------------------------------------------------------------------- */

static bool initialized = false;

static seq_t *term_cache = NULL;   /* cached sequence terms (seq_t type) */
static size_t cache_len   = 0;      /* number of terms stored */
static size_t cache_size  = 0;      /* current allocation size */

static idx_t next_prime_index = 0;  /* index in primes_arr of the next prime to test */

/* Ensure initialisation is done only once. */
static void ensure_initialized(void)
{
    if (!initialized) {
        init_sieve();

        /* Start with a small cache; it will grow as needed. */
        cache_size = 64;
        term_cache = (seq_t *) malloc(cache_size * sizeof(seq_t));
        cache_len  = 0;
        next_prime_index = 0;

        initialized = true;
    }
}

/* Append a new term to the cache, expanding the allocation if required. */
static void cache_append(seq_t value)
{
    if (cache_len == cache_size) {
        cache_size *= 2;
        term_cache = (seq_t *) realloc(term_cache, cache_size * sizeof(seq_t));
    }
    term_cache[cache_len++] = value;
}

/* -------------------------------------------------------------------- */
/*  PUBLIC INTERFACE ------------------------------------------------- */
/* -------------------------------------------------------------------- */

/* Return the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    ensure_initialized();

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* If we run out of pre‑sieved primes, extend the search
           by examining successive integers with trial division. */
        if (next_prime_index >= prime_count) {
            /* Search for the next prime beyond the sieve. */
            idx_t p_test = primes_arr[prime_count - 1] + 2; /* start after last prime */
            for (;;) {
                if (is_prime_trial(p_test)) {
                    /* Append this new prime to the primes array for reuse. */
                    if (prime_count == MAX_PRIME_COUNT) {
                        /* Unlikely to happen within the required range. */
                        break;
                    }
                    primes_arr[prime_count++] = p_test;
                    break;
                }
                p_test += 2; /* skip even numbers */
            }
        }

        idx_t p = primes_arr[next_prime_index++];
        idx_t sp = 2 * p + 1;   /* 2p + 1 */

        /* Skip Sophie Germain primes (where sp is prime). */
        bool sp_is_prime = (sp <= SIEVE_LIMIT) ? is_prime_small(sp) : is_prime_trial(sp);
        if (sp_is_prime) continue;

        /* Least prime divisor of sp. */
        idx_t q = least_prime_factor(sp);

        /* Test condition ((p mod 3) * (q mod 3)) mod 3 == 1. */
        if (((p % 3) * (q % 3)) % 3 == 1) {
            cache_append((seq_t) p);
        }
    }

    return term_cache[n - 1];
}
