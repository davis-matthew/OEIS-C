/*
 * A120416  Numbers k such that k^2+1 divides k!.
 * https://oeis.org/A120416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A120416: numbers k such that k^2+1 divides k! */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----- Prime sieve (once per process) ----- */
static const size_t PRIME_LIMIT = 2000000;   /* enough for n up to about 10^4 */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_list != NULL) return;               /* already done */

    /* sieve */
    bool *is_comp = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    for (size_t p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (!is_comp[p]) {
            for (size_t q = p * p; q <= PRIME_LIMIT; q += p)
                is_comp[q] = true;
        }
    }

    /* count primes */
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) ++prime_count;

    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) prime_list[idx++] = (uint32_t)i;

    free(is_comp);
}

/* ----- Legendre exponent of prime p in k! ----- */
static uint64_t factorial_prime_exp(uint64_t k, uint32_t p)
{
    uint64_t exp = 0;
    uint64_t pp = p;
    while (pp <= k) {
        exp += k / pp;
        if (pp > k / p) break;       /* avoid overflow of pp *= p */
        pp *= p;
    }
    return exp;
}

/* ----- Test whether k satisfies the condition ----- */
static bool good_k(uint64_t k)
{
    uint64_t n = k * k + 1;
    uint64_t m = n;

    /* factor m using prime list */
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= m; ++i) {
        uint32_t p = prime_list[i];
        if (m % p == 0) {
            uint32_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (e > factorial_prime_exp(k, p))
                return false;
        }
    }

    /* remaining factor, if any (must be prime) */
    if (m > 1) {
        uint32_t p = (uint32_t)m;
        if (1 > factorial_prime_exp(k, p))
            return false;
    }
    return true;
}

/* ----- Main sequence accessor ----- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; a(0) unused */

    init_primes();

    uint64_t found = 0;
    uint64_t candidate = 2;
    while (1) {
        if (good_k(candidate)) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
