/*
 * A052292  Primes of form 4*p^2 + 1, p prime.
 * https://oeis.org/A052292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 4*p^2 + 1 for the n‑th prime p such that 4*p^2 + 1 is
 *            prime Sequence: OEIS A052292 – primes of the form 4*p^2 + 1, p
 *            prime The function builds the sequence once, caching the terms
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 416, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------- */
/* Miller–Rabin primality test for 64-bit integers                  */
/* --------------------------------------------------------------- */
static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    /* Small prime divisibility test */
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,
        31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); i++) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    /* Decompose n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        s++;
    }
    /* Deterministic witnesses for 64-bit integers */
    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); i++) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) return true;
        __int128 x = 1;
        __int128 base = a;
        uint64_t exp = d;
        while (exp > 0) {
            if (exp & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* --------------------------------------------------------------- */
/* Globals for the cached sequence                                 */
/* --------------------------------------------------------------- */
#define MAXP 5000000    /* upper bound for prime generation          */
#define INITIAL_SEQ_CAP 25000

static bool initialized = false;
static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0;
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* --------------------------------------------------------------- */
/* Initialization routine – builds primes and the sequence.       */
/* --------------------------------------------------------------- */
static void init_sequence(void) {
    /* Sieve of Eratosthenes up to MAXP */
    uint8_t *is_comp = (uint8_t *)calloc(MAXP + 1, sizeof(uint8_t));
    for (uint32_t i = 2; (uint64_t)i * i <= MAXP; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= MAXP; j += i)
                is_comp[j] = 1;
        }
    }
    /* Build prime list */
    prime_list = (uint32_t *)malloc((MAXP / 2) * sizeof(uint32_t));
    prime_cnt = 0;
    for (uint32_t i = 2; i <= MAXP; ++i)
        if (!is_comp[i])
            prime_list[prime_cnt++] = i;
    free(is_comp);

    /* Allocate cache for sequence values */
    seq_cap = INITIAL_SEQ_CAP;
    seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_len = 0;

    /* Generate the sequence */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        uint64_t pp = (uint64_t)p * p; /* p^2 fits in 64 bits for p <= MAXP */
        uint64_t val = 4ULL * pp + 1ULL;
        if (is_prime_u64(val)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)val;
        }
    }
    initialized = true;
}

/* --------------------------------------------------------------- */
/* Public function required by the exercise                        */
/* --------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;       /* undefined for offset 1, but harmless */
    if (!initialized) init_sequence();
    if (n > seq_len) return 0;  /* fallback – should not happen for n <= 10000 */
    return seq_cache[n - 1];
}
