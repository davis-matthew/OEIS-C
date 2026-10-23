/*
 * A155760  Primes p such that p+30 and p+60 are prime.
 * https://oeis.org/A155760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Search for primes p such that p+30 and p+60 are also prime
 * Range:     u8 n <= 23, u16 n <= 805, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------------- */
/* Sieve parameters */
/* ----------------------------------------------------------------------------- */
static const uint32_t SIEVE_LIMIT = 200000000U;   /* 2×10^8  (≈12.5 MB for odds) */

static bool sieve_initialized = false;
static unsigned char *sieve_bits = NULL;   /* one bit per odd number */
static size_t sieve_bits_len = 0;

/* ----------------------------------------------------------------------------- */
/* Prime list parameters */
/* ----------------------------------------------------------------------------- */
static uint32_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

/* ----------------------------------------------------------------------------- */
/* Sequence storage */
/* ----------------------------------------------------------------------------- */
static seq_t *seq_terms = NULL;
static size_t seq_len = 0, seq_cap = 0;
static size_t prime_iter = 0;            /* index into primes array */

/* ----------------------------------------------------------------------------- */
/* Helper: cast square root of a 32‑bit number (inefficient but trivial) */
/* ----------------------------------------------------------------------------- */
static inline uint32_t isqrt32(uint32_t n)
{
    uint32_t r = 0;
    while ((uint64_t)(r + 1) * (r + 1) <= n) r++;
    return r;
}

/* ----------------------------------------------------------------------------- */
/* Simple bitset manipulate */
/* ----------------------------------------------------------------------------- */
static inline void set_bit(unsigned char *bits, size_t idx)
{
    bits[idx >> 3] |= (1U << (idx & 7));
}
static inline bool test_bit(const unsigned char *bits, size_t idx)
{
    return (bits[idx >> 3] >> (idx & 7)) & 1;
}

/* ----------------------------------------------------------------------------- */
/* Prime test based on the sieve (only odds >2 are stored) */
/* ----------------------------------------------------------------------------- */
static inline bool is_prime_small(uint32_t n)
{
    if (n <= 1) return false;
    if (n == 2) return true;
    if ((n & 1U) == 0) return false;
    size_t idx = n >> 1;
    return test_bit(sieve_bits, idx) == 0;
}

/* ----------------------------------------------------------------------------- */
/* Initialise the sieve and build the prime list (once) */
/* ----------------------------------------------------------------------------- */
static void init_sieve(void)
{
    if (sieve_initialized) return;
    sieve_initialized = true;

    size_t size = (SIEVE_LIMIT >> 1) + 1;          /* only odds */
    sieve_bits = (unsigned char *)calloc(size, sizeof(unsigned char));
    sieve_bits_len = size;

    uint32_t limit = SIEVE_LIMIT;
    uint32_t sqrt_limit = isqrt32(limit);

    for (uint32_t i = 3; i <= sqrt_limit; i += 2)
        if (!test_bit(sieve_bits, i >> 1))
            for (uint32_t j = i * i; j <= limit; j += i << 1)
                set_bit(sieve_bits, j >> 1);

    /* Build the prime list */
    primes_cap = 1000000;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    primes_len = 0;
    primes[primes_len++] = 2;
    for (uint32_t i = 3; i <= limit; i += 2)
        if (!test_bit(sieve_bits, i >> 1)) {
            if (primes_len == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_len++] = i;
        }
}

/* ----------------------------------------------------------------------------- */
/* Grow the sequence storage as needed */
/* ----------------------------------------------------------------------------- */
static void grow_seq(void)
{
    size_t new_cap = seq_cap ? seq_cap << 1 : 256;
    seq_terms = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ----------------------------------------------------------------------------- */
/* Generate terms up to the requested index (1‑based) */
/* ----------------------------------------------------------------------------- */
static void generate_terms(idx_t target)
{
    init_sieve();
    while (seq_len < target) {
        if (prime_iter >= primes_len) break; /* should not happen for n ≤ 10 000 */
        uint32_t p = primes[prime_iter++];
        if ((uint64_t)p + 60U > SIEVE_LIMIT) break;
        if (is_prime_small(p) && is_prime_small(p + 30U)
                && is_prime_small(p + 60U)) {
            if (seq_len == seq_cap) grow_seq();
            seq_terms[seq_len++] = (seq_t)p;
        }
    }
}

/* ----------------------------------------------------------------------------- */
/* Public function required by the interface */
/* ----------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for internal use */
    if (seq_len < (size_t)n) generate_terms(n);
    if ((size_t)n > seq_len) return 0;    /* if limit exceeded */
    return seq_terms[n - 1];
}
