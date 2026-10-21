/*
 * A090259  Least even a(n) > 2 not representable as a sum of two of the first
 *          n primes. From Goldbach conjecture.
 * https://oeis.org/A090259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 3535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   estimate_upper_bound_for_nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A090259: least even > 2 not representable as a sum of two of the first n primes */
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "../utils/estimate_upper_bound_for_nth_prime.h"

#ifndef MAX_SIEVE_LIMIT
#define MAX_SIEVE_LIMIT 500000   /* enough for n up to ~40000 */
#endif

static unsigned char is_prime[MAX_SIEVE_LIMIT + 1];
static int primes[MAX_SIEVE_LIMIT];
static int prime_count = 0;
static unsigned sieve_limit = 0;

/* Build sieve and prime list up to 'limit' (inclusive) */
static void build_sieve(unsigned limit)
{
    if (limit <= sieve_limit)
        return;

    if (limit > MAX_SIEVE_LIMIT) {
        /* exceed preallocated static buffer; you may want to abort or resize */
        return;
    }

    for (unsigned i = 2; i <= limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    unsigned sqrt_limit = (unsigned)floor(sqrt((double)limit));
    for (unsigned i = 2; i <= sqrt_limit; ++i) {
        if (is_prime[i]) {
            for (unsigned j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    prime_count = 0;
    for (unsigned i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes[prime_count++] = (int)i;

    sieve_limit = limit;
}

/* Ensure we have at least the first 'need' primes available */
static void ensure_primes(unsigned need)
{
    if ((unsigned)prime_count >= need)
        return;

    unsigned limit = estimate_upper_bound_for_nth_prime(need);
    if (limit > sieve_limit)
        build_sieve(limit);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined offset, but guard */

    unsigned ui = (unsigned)n;

    /* prepare primes */
    ensure_primes(ui);

    /* limit for search: 2*p(n)+2 */
    uint64_t p_n = (uint64_t)primes[ui - 1];
    uint64_t search_limit = 2 * p_n + 2;
    if ((unsigned)search_limit > sieve_limit)
        build_sieve((unsigned)search_limit);

    /* search for the least even > 2 that cannot be written as p_i + p_j (i,j < n) */
    for (uint64_t e = 4; e <= search_limit; e += 2) {
        unsigned found = 0;
        for (unsigned i = 0; i < ui && (uint64_t)primes[i] <= e / 2; ++i) {
            uint64_t other = e - (uint64_t)primes[i];
            if (other <= p_n && (unsigned)other <= sieve_limit && is_prime[(unsigned)other]) {
                found = 1;
                break;
            }
        }
        if (!found)
            return (seq_t)e;
    }

    /* Should not reach here because of the bound */
    return (seq_t)search_limit;
}
