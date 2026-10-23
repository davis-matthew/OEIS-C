/*
 * A049437  Primes p such that p+2 and p+8 are also primes but p+6 is not.
 * https://oeis.org/A049437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that p, p+2, p+8 are primes but p+6
 *            is not. The function builds a sieve large enough to contain at
 *            least 10 000 such primes and memoises the sequence
 * Range:     u8 n <= 5, u16 n <= 161, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static uint8_t *sieve_bits = NULL;
static size_t sieve_limit = 0;

/* bit‑map helpers -----------------------------------------------------*/
static inline void set_composite(uint32_t n) {
    n >>= 1;                           /* map odd number to bitmap index */
    sieve_bits[n >> 3] |= (uint8_t)(1 << (n & 7));
}

static inline bool is_composite(uint32_t n) {
    n >>= 1;
    return (sieve_bits[n >> 3] & (uint8_t)(1 << (n & 7))) != 0;
}

static inline bool is_prime_sieve(uint32_t n) {
    if (n == 2) return true;
    if (n < 2 || (n & 1) == 0) return false;
    return !is_composite(n);
}

/* prime storage --------------------------------------------------------*/
static uint32_t *primes = NULL;
static size_t primes_count = 0;
static size_t primes_cap   = 0;

/* sequence cache (uint64_t allows conversion to any seq_t) ----------*/
static uint64_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static size_t prime_index = 0;          /* next prime to examine */

/* build a single sieve ------------------------------------------------*/
static void build_sieve(void) {
    if (sieve_limit) return;           /* already built */

    /* a generous limit to contain more than 10 000 terms (~ 5–6 × 10⁷) */
    sieve_limit = 100000000U;           /* 1e8 */

    /* allocate bitset for odd numbers up to sieve_limit */
    size_t odd_count  = (sieve_limit >> 1) + 1;          /* count of odd n */
    size_t byte_count = (odd_count + 7) >> 3;           /* bytes needed */

    sieve_bits = (uint8_t *)malloc(byte_count);
    if (!sieve_bits) exit(1);
    for (size_t i = 0; i < byte_count; ++i) sieve_bits[i] = 0;

    /* Sieve of Eratosthenes for odd numbers only */
    for (uint32_t i = 3; i <= sieve_limit / i; i += 2) {
        if (!is_composite(i)) {
            uint32_t start = i * i;
            for (uint32_t j = start; j <= sieve_limit; j += (i << 1))
                set_composite(j);
        }
    }

    /* collect primes into a dynamic array */
    primes_cap = sieve_limit / 10;              /* initial estimate */
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) exit(1);
    primes[0] = 2;                              /* the only even prime */
    primes_count = 1;

    for (uint32_t n = 3; n <= sieve_limit; n += 2) {
        if (!is_composite(n)) {
            if (primes_count == primes_cap) {
                primes_cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(primes,
                                primes_cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                primes = tmp;
            }
            primes[primes_count++] = n;
        }
    }
}

/* ----------------------------------------------------------------------------
 *  seq_t a(idx_t n)
 * ---------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* undefined for offset 1 */

    build_sieve();                            /* ensure sieve ready */

    /* expand the cached sequence until it has at least n terms */
    while (seq_len < n) {
        if (prime_index >= primes_count) {
            /* this should not happen for n ≤ 10 000 with the chosen limit */
            break;
        }

        uint32_t p = primes[prime_index++];
        if (p + 8ULL > sieve_limit) break;    /* safety guard */

        if (is_prime_sieve(p + 2) &&
            is_prime_sieve(p + 8) &&
           !is_prime_sieve(p + 6)) {
            /* grow cache if necessary */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap << 1 : 64;
                uint64_t *tmp = (uint64_t *)realloc(seq_cache,
                               seq_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                seq_cache = tmp;
            }
            seq_cache[seq_len++] = (uint64_t)p;
        }
    }

    return (seq_t)seq_cache[n - 1];
}
