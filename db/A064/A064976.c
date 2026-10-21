/*
 * A064976  n*10^2-1, n*10^2-3, n*10^2-7 and n*10^2-9 are all prime.
 * https://oeis.org/A064976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 64, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064976
   a(n) is the n‑th integer m such that
   m*100-1, m*100-3, m*100-7 and m*100-9 are all prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime sieve (up to a fixed limit) ---------- */
#define PRIME_LIMIT 1000000               /* enough to factor all values we test */

static uint32_t *prime_list = NULL;       /* array of primes */
static size_t   prime_count = 0;

/* allocate once the list of primes up to PRIME_LIMIT */
static void init_primes(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    bool *is_comp = (bool*)calloc(PRIME_LIMIT + 1U, sizeof(bool));
    if (!is_comp) abort();               /* unrecoverable error */

    size_t prime_cap = 80000;            /* rough upper bound for π(P) */
    prime_list = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_list) abort();

    for (uint32_t i = 2U; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_count == prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint32_t*)realloc(prime_list,
                                                prime_cap * sizeof(uint32_t));
                if (!prime_list) abort();
            }
            prime_list[prime_count++] = i;
            if ((uint64_t)i * (uint64_t)i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

/* ---------- Deterministic primality test ---------- */
static inline bool is_prime(uint64_t n)
{
    if (n < 2U) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) return n == (uint64_t)p;
    }
    return true;
}

/* ---------- Cache of found terms ---------- */
static seq_t *seq_terms = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static idx_t  next_t    = 2U;          /* next candidate integer to test */

/* make room for at least one more term in the cache */
static void append_term(seq_t value)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap == 0 ? 4U : seq_cap << 1;
        seq_terms = (seq_t*)realloc(seq_terms, seq_cap * sizeof(seq_t));
        if (!seq_terms) abort();
    }
    seq_terms[seq_len++] = value;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0;      /* offset is 1, but allow n=0 harmlessly */

    init_primes();                     /* ensure prime list prepared */

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        uint64_t t   = (uint64_t)next_t;
        uint64_t v0  = t * 100ULL - 1ULL;
        uint64_t v1  = t * 100ULL - 3ULL;
        uint64_t v2  = t * 100ULL - 7ULL;
        uint64_t v3  = t * 100ULL - 9ULL;

        if (is_prime(v0) && is_prime(v1) && is_prime(v2) && is_prime(v3)) {
            append_term((seq_t)t);
        }
        ++next_t;
    }
    return seq_terms[(size_t)n - 1];
}
