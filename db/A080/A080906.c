/*
 * A080906  Primes with an even number of digits such that the first half of
 *          the digits and the second half of the digits are both primes.
 * https://oeis.org/A080906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 96, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes with an even number of digits such that the
   first half of the digits and the second half are both primes.
   Computed by enumerating successive primes and selecting those
   whose two halves are themselves prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define SMALL_PRIME_LIMIT 1000000

/* ----- small sieve for numbers <= SMALL_PRIME_LIMIT ----- */
static bool *sieve_small = NULL;

static void init_sieve_small(void) {
    if (sieve_small) return;
    sieve_small = (bool*)calloc(SMALL_PRIME_LIMIT + 1, 1);
    if (!sieve_small) return; /* allocation failed – will not happen in tests */
    for (int i = 2; i <= SMALL_PRIME_LIMIT; ++i) sieve_small[i] = true;
    for (int i = 2; (int64_t)i * i <= SMALL_PRIME_LIMIT; ++i) {
        if (sieve_small[i]) {
            for (int j = i * i; j <= SMALL_PRIME_LIMIT; j += i)
                sieve_small[j] = false;
        }
    }
}

static inline bool small_is_prime(uint32_t n) {
    if (n <= SMALL_PRIME_LIMIT) return sieve_small[n];
    return false; /* will be tested by Miller‑Rabin */
}

/* ----- modular multiplication and exponentiation for 64‑bit ----- */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t pow_mod(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return r;
}

/* ----- deterministic Miller‑Rabin for 64‑bit integers ----- */
static bool is_prime64(uint64_t n) {
    if (n < 2) return false;
    if (n <= SMALL_PRIME_LIMIT) return small_is_prime((uint32_t)n);

    /* quick check small primes */
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* Miller‑Rabin with deterministic bases for 64‑bit */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ----- helper to count decimal digits of a 64‑bit number ----- */
static inline int decimal_digits(uint64_t v) {
    int d = 0;
    do { v /= 10; ++d; } while (v);
    return d;
}

/* ----- main sequence access function ----- */
seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *terms = NULL;          /* array of found terms */
    static idx_t term_count = 0;         /* number of terms stored */
    static uint64_t cur_num = 23;        /* next candidate to test */

    /* initialise shared resources once */
    init_sieve_small();

    /* generate terms until we have at least n of them */
    while (term_count < n) {
        /* find next prime */
        while (!is_prime64(cur_num)) ++cur_num;

        /* even number of digits required */
        int digits = decimal_digits(cur_num);
        if ((digits & 1) == 0) {
            int half = digits >> 1;
            uint64_t pow10 = 1;
            for (int i = 0; i < half; ++i) pow10 *= 10;
            uint64_t first = cur_num / pow10;
            uint64_t second = cur_num % pow10;

            if (is_prime64(first) && is_prime64(second)) {
                /* store the term in the array */
                if (term_count + 1 > term_count) {
                    size_t new_cap = term_count ? term_count * 2 : 32;
                    terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
                }
                terms[term_count++] = (seq_t)cur_num;
            }
        }
        ++cur_num;
    }

    return terms[n - 1];
}
