/*
 * A056777  Composite numbers k such that both phi(k+12) = phi(k) + 12 and
 *          sigma(k+12) = sigma(k) + 12.
 * https://oeis.org/A056777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 28, u64 n <= 166, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = p*(p+8), where p, p+2, p+6, p+8 are consecutive primes
 * (a prime quadruplet).  The sequence starts with p = 5, producing 65, 209,
 * 11009, ...
 *
 * Implementation: precompute all prime quadruplets up to a fixed limit
 * (2×10^8) using a simple bitset sieve.  The resulting terms are stored
 * in a static array and returned by index.
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static const uint64_t SIEVE_LIMIT = 200000000ULL; /* 2·10^8 */
static uint8_t *prime_bits = NULL;                /* bitset of composites */
static seq_t *terms = NULL;                      /* sequence values */
static size_t terms_len = 0;
static size_t terms_cap = 0;

static inline void set_composite(uint64_t n)
{
    prime_bits[n >> 3] |= (uint8_t)(1U << (n & 7));
}
static inline int is_composite(uint64_t n)
{
    return prime_bits[n >> 3] & (1U << (n & 7));
}
static inline int is_prime64(uint64_t n)
{
    return n >= 2 && !is_composite(n);
}

/* Simple sieve up to SIEVE_LIMIT */
static void init_sieve(void)
{
    size_t bytes = (SIEVE_LIMIT >> 3) + 1;
    prime_bits = (uint8_t *)malloc(bytes);
    memset(prime_bits, 0, bytes);
    set_composite(0);
    set_composite(1);

    for (uint64_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!is_composite(i)) {
            for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i)
                set_composite(j);
        }
    }
}

/* Generate all terms up to the sieve limit */
static void generate_terms(void)
{
    if (terms_len > 0)
        return;          /* already generated */
    if (!prime_bits)
        init_sieve();

    terms_cap = 20000;   /* initial capacity */
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));

    for (uint64_t p = 5; p + 8 <= SIEVE_LIMIT; p += 2) {
        if (!is_composite(p) &&
            !is_composite(p + 2) &&
            !is_composite(p + 6) &&
            !is_composite(p + 8)) {
            seq_t val = (seq_t)p * (seq_t)(p + 8);
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms,
                                         terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = val;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* out of bounds, undefined */

    generate_terms();

    if ((idx_t)n > terms_len)
        return 0;               /* n too large for the precomputed range */

    return terms[(idx_t)n - 1];
}
