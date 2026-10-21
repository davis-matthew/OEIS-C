/*
 * A078851  Initial term in sequence of four consecutive primes separated by 3
 *          consecutive differences each <=6 (i.e., when d=2,4 or 6) and
 *          forming d-pattern=[4, 6, 2]; short d-string notation of pattern =
 *          [462].
 * https://oeis.org/A078851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 55, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078851:
   Find primes p such that p+4, p+10, and p+12 are also prime.
   The sequence lists the initial prime p of each such quadruple. */
  
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Small prime list for trial division (up to 1000). */
static const uint32_t SMALL_PRIME_LIMIT = 1000;
static uint32_t *small_primes = NULL;
static size_t small_primes_cnt = 0;

/* State for generating primes in order. */
static uint64_t last_processed_prime = 0;
static uint64_t last[3] = {0, 0, 0};
static size_t last_cnt = 0;

/* Cached terms of the sequence. */
static seq_t *terms = NULL;
static size_t terms_cnt = 0;
static size_t terms_cap = 0;

/* -------------------------------------------------------------- */
/* Helper: initialise small primes table (sieve up to 1000).       */
static void init_small_primes(void) {
    if (small_primes_cnt) return;          /* already done */
    size_t sz = SMALL_PRIME_LIMIT + 1;
    bool *is_composite = (bool*)calloc(sz, sizeof(bool));
    small_primes = (uint32_t*)malloc(200 * sizeof(uint32_t));
    small_primes_cnt = 0;
    for (uint32_t i = 2; i <= SMALL_PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            small_primes[small_primes_cnt++] = i;
            if ((uint64_t)i * i <= SMALL_PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= SMALL_PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
}

/* -------------------------------------------------------------- */
/* Modular multiplication using 128‑bit if available.             */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

/* Modular exponentiation. */
static uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) res = mul_mod(res, base, mod);
        base   = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* Deterministic Miller–Rabin test for 64-bit integers. */
static bool is_prime_mr(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Quick primality test using small primes followed by MR. */
static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    for (size_t i = 0; i < small_primes_cnt; ++i) {
        uint32_t p = small_primes[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return n == p;
    }
    return is_prime_mr(n);
}

/* Return the next prime greater than curr. */
static uint64_t next_prime(uint64_t curr) {
    uint64_t cand = curr + 1;
    if (cand <= 2) return 2;
    if (cand % 2 == 0) ++cand;
    for (;;) {
        if (is_prime(cand)) return cand;
        cand += 2;
    }
}

/* Store a new term into the cache. */
static void append_term(seq_t val) {
    if (terms_cnt >= terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 64;
        terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_cnt++] = val;
}

/* -------------------------------------------------------------- */
/* Public sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;      /* undefined for indices < 1 */
    init_small_primes();

    /* Generate terms until we have at least n of them. */
    while (terms_cnt < (size_t)n) {
        uint64_t p = next_prime(last_processed_prime);
        last_processed_prime = p;

        if (last_cnt < 3) {            /* accumulate first three primes */
            last[last_cnt++] = p;
        } else {                       /* window of last 3 primes + p */
            uint64_t p0 = last[0];
            uint64_t p1 = last[1];
            uint64_t p2 = last[2];
            if (p1 == p0 + 4 && p2 == p1 + 6 && p == p2 + 2) {
                append_term((seq_t)p0);   /* record the starting prime */
            }
            /* shift window */
            last[0] = last[1];
            last[1] = last[2];
            last[2] = p;
        }
    }
    return terms[n - 1];
}
