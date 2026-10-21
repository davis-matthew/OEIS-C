/*
 * A088071  Number sandwiched between two numbers having only one prime
 *          divisor.
 * https://oeis.org/A088071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088071: numbers n such that n‑1 and n+1 are prime powers. */
/* The implementation builds a sieve of primes up to a fixed limit, */
/* then scans successive integers, testing whether both neighbours are prime powers. */
/* It caches discovered terms in a dynamic array for efficient repeated calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* ---------- global state for primes ---------- */
static uint32_t *sieve_primes = NULL;   /* array of prime numbers */
static size_t primes_len   = 0;          /* number of primes stored */
static size_t primes_cap   = 0;          /* current allocation capacity */
static bool   primes_inited = false;     /* whether the sieve has been built */

/* ---------- sieve of primes up to 'limit' (inclusive) ---------- */
static void init_primes(uint32_t limit)
{
    static bool *is_prime   = NULL;
    static size_t is_prime_cap = 0;

    /* realloc if necessary */
    if (is_prime_cap < (size_t)limit + 1) {
        if (is_prime) free(is_prime);
        is_prime = (bool*)malloc((size_t)limit + 1);
        is_prime_cap = (size_t)limit + 1;
    }

    /* initialise all entries to prime */
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* enlarge prime list container if needed */
    if (primes_cap < (size_t)limit) {
        uint32_t *tmp = (uint32_t*)realloc(sieve_primes, (size_t)limit * sizeof(uint32_t));
        sieve_primes = tmp;
        primes_cap   = (size_t)limit;
    }

    /* collect primes */
    primes_len = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            sieve_primes[primes_len++] = i;
}

/* ---------- test if a number is a prime power ---------- */
static bool is_prime_power(seq_t n)
{
    if (n < 2) return false;          /* 0 and 1 are not prime powers */

    seq_t m = n;
    for (size_t i = 0; i < primes_len; ++i) {
        uint32_t p = sieve_primes[i];
        seq_t pp = (seq_t)p * (seq_t)p;
        if (pp > m) break;           /* no need to check larger primes */

        if (m % p == 0) {
            /* divide out all copies of p */
            while (m % p == 0)
                m /= p;
            /* remaining factor must be 1 for a prime power */
            return m == 1;
        }
    }
    /* m had no small prime factor → it is prime → a prime power */
    return true;
}

/* ---------- dynamic array for cached sequence terms ---------- */
static seq_t *seq_arr = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_arr = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* initialise the prime table once */
    if (!primes_inited) {
        /* a generous upper bound: sufficient for n up to ~10 000 */
        init_primes(20000000U);          /* 20 million */
        primes_inited = true;
    }

    /* static state for the next candidate to test */
    static seq_t next_candidate = 3;    /* the first term in the sequence */

    /* generate terms until we have at least 'n' of them */
    while (seq_len < n) {
        seq_t cand = next_candidate;

        if (is_prime_power(cand - 1) && is_prime_power(cand + 1)) {
            ensure_seq_capacity(seq_len + 1);
            seq_arr[seq_len++] = cand;
        }

        ++next_candidate;
    }

    return seq_arr[n - 1];
}
