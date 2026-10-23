/*
 * A109835  Primes connected to two primes by the (p+1)/2 and 2p-1 operators.
 * https://oeis.org/A109835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p such that p is prime, (p+1)/2 is prime, and 2p-1 is
 *            prime
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Limit for sieving (must be at least twice the largest required prime). */
#define SIEVE_LIMIT 200000000ULL   /* 200‑million */

/* Global state needed for lazy initialization */
static bool initialized = false;

/* Bit set for odd numbers only; bit i corresponds to number 2*i+1 */
static uint8_t *sieve_bits = NULL;
static size_t sieve_bits_size = 0;

/* Array of primes that satisfy the condition */
static uint64_t *special_primes = NULL;
static size_t special_count = 0;
static size_t special_cap = 0;

/* Helper functions for bit operations on sieve_bits */
static inline bool bit_is_set(size_t idx) {
    return (sieve_bits[idx >> 3] >> (idx & 7)) & 1U;
}

static inline void bit_set(size_t idx) {
    sieve_bits[idx >> 3] |= (uint8_t)(1U << (idx & 7));
}

/* Resize dynamic array for special primes */
static void add_special_prime(uint64_t val) {
    if (special_count >= special_cap) {
        size_t new_cap = special_cap ? special_cap << 1 : 128;
        uint64_t *tmp = (uint64_t *)realloc(special_primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort(); /* memory failure */
        special_primes = tmp;
        special_cap = new_cap;
    }
    special_primes[special_count++] = val;
}

/* Return true if x <= SIEVE_LIMIT and is prime according to sieve_bits */
static bool is_prime_bit(uint64_t x) {
    if (x == 2) return true;
    if ((x & 1U) == 0) return false;
    if (x > SIEVE_LIMIT) return false;
    return !bit_is_set(x >> 1);
}

/* Initialise sieve and build the list of special primes */
static void init(void) {
    if (initialized) return;
    initialized = true;

    /* Prepare sieve bit array */
    size_t bits_needed = (SIEVE_LIMIT >> 1) + 1;              /* odd numbers only */
    sieve_bits_size = (bits_needed >> 3) + 1;
    sieve_bits = (uint8_t *)calloc(sieve_bits_size, sizeof(uint8_t));
    if (!sieve_bits) abort();

    /* Sieve of Eratosthenes for odd numbers */
    for (uint64_t i = 3; i * i <= SIEVE_LIMIT; i += 2) {
        if (!bit_is_set(i >> 1)) {
            for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i << 1) {
                bit_set(j >> 1);
            }
        }
    }

    /* Build list of special primes */
    special_primes = NULL;  /* will be allocated by add_special_prime */
    special_cap = 0;
    special_count = 0;

    /* Check prime 2 (not relevant because (2+1)/2 = 1.5 not integer) */

    /* Iterate over odd primes up to SIEVE_LIMIT */
    for (uint64_t p = 3; p <= SIEVE_LIMIT; p += 2) {
        if (!bit_is_set(p >> 1)) {                    /* p is prime */
            uint64_t q = (p + 1) >> 1;                /* (p+1)/2 */
            if (!is_prime_bit(q))
                continue;                            /* q not prime */

            uint64_t r = (p << 1) - 1;                /* 2p-1 */
            if (r > SIEVE_LIMIT)                     /* r beyond sieve */
                continue;
            if (!is_prime_bit(r))
                continue;                            /* r not prime */

            add_special_prime(p);
        }
    }
}

/* External function as required by the problem statement */
seq_t a(idx_t n) {
    init();  /* Ensure initialization once */

    /* n is 1‑based */
    if (n == 0 || (idx_t)special_count < n) {
        /* Out of range – return 0 (could also abort or handle differently) */
        return (seq_t)0;
    }

    return (seq_t)special_primes[n - 1];
}
