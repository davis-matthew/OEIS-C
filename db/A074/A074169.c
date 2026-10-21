/*
 * A074169  Number of representations of n as a sum of two primes that are not
 *          congruent modulo 3.
 * https://oeis.org/A074169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes
   a(n) = #{ unordered prime pairs (p,q) | p+q = n , p%3 ≠ q%3 }.
   It uses a simple sieve to generate all primes up to n. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint8_t  *prime_flag = NULL;   /* 0 = composite, 1 = prime */
static size_t    prime_flag_size = 0; /* allocated size of prime_flag  */
static uint32_t *prime_list  = NULL;  /* list of primes (up to current limit) */
static size_t    prime_count  = 0;    /* number of primes stored */
static size_t    prime_cap    = 0;    /* capacity of prime_list       */
static size_t    sieve_limit  = 0;    /* highest value sieved so far   */

/* Expand memory for prime_flag to at least 'size'. */
static void expand_prime_flag(size_t size)
{
    if (prime_flag_size >= size) return;
    size_t new_size = prime_flag_size == 0 ? 16 : prime_flag_size;
    while (new_size < size) new_size <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(prime_flag, new_size);
    if (!tmp) abort();              /* out of memory – terminate */
    prime_flag = tmp;
    prime_flag_size = new_size;
}

/* Expand memory for prime_list to at least 'cap' entries. */
static void expand_prime_list(size_t cap)
{
    if (prime_cap >= cap) return;
    size_t new_cap = prime_cap == 0 ? 16 : prime_cap;
    while (new_cap < cap) new_cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap = new_cap;
}

/* Re‑sieve up to 'limit' (new limit is always ≥ previous limit). */
static void extend_sieve(size_t limit)
{
    if (limit <= sieve_limit) return;   /* already enough */

    /* allocate flag array */
    expand_prime_flag(limit + 1);

    /* initialise all entries to true (1) */
    for (size_t i = 0; i <= limit; ++i) prime_flag[i] = 1;

    if (limit >= 0) prime_flag[0] = 0;
    if (limit >= 1) prime_flag[1] = 0;

    /* classic Sieve of Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (prime_flag[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                prime_flag[j] = 0;
            }
        }
    }

    /* rebuild prime list */
    size_t needed = 0;
    for (size_t i = 2; i <= limit; ++i) if (prime_flag[i]) ++needed;
    expand_prime_list(needed);
    prime_count = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (prime_flag[i]) {
            prime_list[prime_count++] = (uint32_t)i;
        }
    }

    sieve_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 4) return 0;  /* smallest representation is 2+3 = 5 */

    /* make sure primes up to n are available */
    extend_sieve((size_t)n);

    seq_t result = 0;
    size_t half = n / 2;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if (p > half) break;                /* ensure p <= q */
        uint32_t q = (uint32_t)(n - p);
        if (q < p) continue;                /* enforce unorderedness */
        if (!prime_flag[q]) continue;        /* q must be prime */

        /* check congruence modulo 3 */
        if ((p % 3) != (q % 3)) {
            result++;
        }
    }

    return result;
}
