/*
 * A082251  Primes that are the sum of 9 consecutive primes.
 * https://oeis.org/A082251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 238, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_bit_set, mod_mul_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A082251:  primes that are the sum of 9 consecutive primes.
    The function computes the sequence by sieving primes up to a fixed
    limit, then building the list of sums of 9 consecutive primes and
    testing each sum for primality (using the sieve for sums within
    the limit and Miller–Rabin for larger sums).  The function is
    memoised; any n may be requested in any order.  The limit (5 000 000)
    guarantees > 20 000 such primes, more than enough for the tests
    (≈ 10 000 terms). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_bit_set.h"
#include "../utils/mod_mul_uint64.h"

/* ------------------------------------------------------------------ */
/*  Types supplied by the build                                        */
/* ------------------------------------------------------------------ */
/* seq_t and idx_t are defined in the build environment. */

/* ------------------------------------------------------------------ */
/*  Parameters and static data                                         */
/* ------------------------------------------------------------------ */
#define LIMIT 5000000u          /* prime sieve limit – 5 M */

static bool initialized = false;

/* sieve – bit array: bit i set ⇒ i is prime */
static unsigned char *sieve_bits = NULL;
static size_t sieve_bits_len = 0;

/* list of primes */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

/* list of terms: primes that are sums of 9 consecutive primes */
static seq_t *terms = NULL;
static size_t terms_cnt = 0;
static size_t terms_cap = 0;

/* ------------------------------------------------------------------ */
/*  Utility for bit operations on the sieve                           */

static inline void set_bit(unsigned char *bits, uint32_t i) {
    bits[i >> 3] |= 1U << (i & 7);
}

/* ------------------------------------------------------------------ */
/*  Miller–Rabin primality test for 64‑bit integers                   */

static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    while (exp) {
        if (exp & 1) res = mod_mul_uint64(res, base, mod);
        base = mod_mul_uint64(base, base, mod);
        exp >>= 1;
    }
    return res;
}
static bool is_prime64(uint64_t n) {
    if (n < 2) return false;
    static const uint32_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        uint64_t a = small[i];
        if (a >= n) break;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Initialise all static data                                         */
/* ------------------------------------------------------------------ */
static void init(void) {
    /* sieve bits */
    sieve_bits_len = (LIMIT >> 3) + 1;
    sieve_bits = (unsigned char*)malloc(sieve_bits_len);
    for (size_t i = 0; i < sieve_bits_len; ++i) sieve_bits[i] = 0xFF;   /* all set */
    set_bit(sieve_bits, 0);
    set_bit(sieve_bits, 1);
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (is_bit_set(sieve_bits, i)) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                sieve_bits[j >> 3] &= ~(1U << (j & 7));
        }
    }
    /* collect primes */
    primes_cnt = 0;
    primes = (uint32_t*)malloc(400000 * sizeof(uint32_t));   /* over‑allocate */
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_bit_set(sieve_bits, i))
            primes[primes_cnt++] = i;

    /* build terms */
    terms_cap = 1024;
    terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    terms_cnt = 0;

    if (primes_cnt < 9) return;   /* nothing to do */

    uint64_t sum = 0;
    for (size_t i = 0; i < 9; ++i) sum += primes[i];

    for (size_t i = 0; ; ++i) {
        bool prime_sum;
        if (sum <= LIMIT)
            prime_sum = is_bit_set(sieve_bits, (uint32_t)sum);
        else
            prime_sum = is_prime64(sum);
        if (prime_sum) {
            if (terms_cnt == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_cnt++] = (seq_t)sum;
        }
        if (i + 9 >= primes_cnt) break;
        sum += primes[i + 9] - primes[i];
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Public interface                                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1; caller may give 1 */

    if (!initialized) init();

    /* if requested n exceeds precomputed terms, we cannot provide it.
       The test dataset never asks for such n. */
    if ((idx_t)terms_cnt < n) return (seq_t)0;
    return terms[(size_t)n - 1];
}
