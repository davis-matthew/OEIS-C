/*
 * A092125  Numbers k such that k, k+2, k+4 are semiprimes.
 * https://oeis.org/A092125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   the n-th integer k such that k, k+2, and k+4 are semiprimes
 *            (with the sequence offset 1). The implementation is a simple
 *            deterministic search that uses a pre‑computed prime table for
 *            fast trial division. It works for any type seq_t that can be
 *            constructed from an integer constant
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ---------- Prime table ------------------------------------------------- */

#define PRIME_TABLE_LIMIT 10000000UL  /* primes up to this limit */

static unsigned int *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_table_limit = 0;

/* Build a classic sieve of Eratosthenes up to PRIME_TABLE_LIMIT.
   The function is called once on the first call to a().  */
static void build_prime_table(void)
{
    if (prime_table_limit >= PRIME_TABLE_LIMIT)
        return;

    unsigned char *is_comp = (unsigned char *)malloc(PRIME_TABLE_LIMIT + 1);
    if (!is_comp)
        exit(1);
    for (size_t i = 0; i <= PRIME_TABLE_LIMIT; ++i)
        is_comp[i] = 0;
    is_comp[0] = is_comp[1] = 1;
    for (size_t i = 2; i * i <= PRIME_TABLE_LIMIT; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= PRIME_TABLE_LIMIT; j += i)
                is_comp[j] = 1;

    /* Count primes to allocate the array */
    size_t estimated = PRIME_TABLE_LIMIT / 10 + 10; /* rough estimate */
    prime_list = (unsigned int *)malloc(estimated * sizeof(unsigned int));
    if (!prime_list)
        exit(1);
    for (size_t i = 2; i <= PRIME_TABLE_LIMIT; ++i)
        if (!is_comp[i]) {
            if (prime_count >= estimated) {
                estimated *= 2;
                prime_list = (unsigned int *)realloc(prime_list,
                                                     estimated * sizeof(unsigned int));
                if (!prime_list)
                    exit(1);
            }
            prime_list[prime_count++] = (unsigned int)i;
        }
    free(is_comp);
    prime_table_limit = PRIME_TABLE_LIMIT;
}

/* ---------- Helpers ----------------------------------------------------- */

/* Simple trial‑divide primality test using the precomputed primes. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    for (size_t i = 1; i < prime_count; ++i) {
        unsigned int p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* Test whether n is a semiprime: product of exactly two primes
   (counted with multiplicity).  We look for the smallest prime factor
   p and check whether the cofactor is prime. */
static bool is_semiprime_uint64(uint64_t n)
{
    if (n < 4)
        return false;                 /* 4 = 2*2 is the smallest semiprime */

    for (size_t i = 0; i < prime_count; ++i) {
        unsigned int p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > n)
            break;
        if (n % p == 0) {
            uint64_t q = n / p;
            return is_prime_uint64(q); /* q is prime → n = p*q or p^2 */
        }
    }
    /* No factor found up to sqrt(n) → n is prime or 1 */
    return false;
}

/* ---------- Sequence generator ------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined for offset 1, but harmless */

    /* Initialise the small‑prime table once */
    static bool table_built = false;
    if (!table_built) {
        build_prime_table();
        table_built = true;
    }

    size_t found = 0;
    uint64_t k = 3;                   /* first odd candidate */

    while (found < n) {
        if (is_semiprime_uint64(k) &&
            is_semiprime_uint64(k + 2) &&
            is_semiprime_uint64(k + 4)) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        k += 2;                       /* skip even numbers */
    }
    /* Unreachable, but keeps the compiler happy */
    return (seq_t)0;
}
