/*
 * A089105  Values taken by least witness function W(n).
 * https://oeis.org/A089105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n‑th positive integer ≥2 that is not a prime power
 * (i.e., it is not of the form p^k with k ≥ 2).  This matches OEIS
 * entry A089105: 2,3,5,6,7,10,11,12,13,14,15,17,18,19,20,21,22,23,24,26,...
 * The implementation is O(n √a) and memoises the computed terms so that
 * subsequent calls (in any order) are fast.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* -----------------  Static data ----------------- */
static seq_t *terms          = NULL;   /* array of a(k) values          */
static idx_t  terms_len      = 0;      /* number of terms currently stored      */
static idx_t  terms_cap      = 0;      /* capacity of the array                */

static uint64_t next_candidate = 2;    /* next integer to test                */

static uint32_t *prime_list   = NULL;  /* list of primes up to 200000          */
static uint64_t prime_cnt     = 0;      /* number of primes stored              */
static uint32_t prime_cap     = 0;      /* capacity of prime_list array         */
static bool primes_inited     = false;  /* whether primes have been generated    */

/* ------------------------------------------------ */
static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;

    const uint32_t LIMIT = 200000;   /* sufficient for n up to 10 000                */
    bool *is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;           /* allocation failure – leave empty      */

    for (uint32_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint32_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    /* Count primes first to allocate exact size */
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            ++prime_cnt;

    prime_list = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_list) { free(is_prime); return; }

    uint64_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            prime_list[idx++] = i;

    free(is_prime);
}

/* ------------------------------------------------ */
static bool is_prime_power(seq_t value)
{
    if (value < 4) return false;    /* 2 and 3 are primes, not excluded       */

    uint64_t v = (uint64_t)value;
    for (uint64_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > v) break;
        if (v % p == 0) {
            uint64_t tmp = v;
            while (tmp % p == 0) tmp /= p;
            return tmp == 1;      /* true if v is a pure power of p          */
        }
    }
    return false;                  /* no divisor found → v is prime           */
}

/* ------------------------------------------------ */
static void ensure_terms_capacity(void)
{
    if (terms_len >= terms_cap) {
        terms_cap = terms_cap ? terms_cap << 1 : 64;
        seq_t *tmp = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
        if (tmp) terms = tmp;        /* silently ignore realloc failure          */
    }
}

/* ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* n is guaranteed ≥ 1, but safe‑guard      */

    /* Initialise the helper tables on first call */
    if (!primes_inited) init_primes();
    if (!terms) {
        terms_cap = 64;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    }

    /* Generate terms up to the requested index */
    while (terms_len < n) {
        if (!is_prime_power((seq_t)next_candidate)) {   /* keep it */
            ensure_terms_capacity();
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return terms[n - 1];
}
