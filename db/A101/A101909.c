/*
 * A101909  Number of primes between 2n and 4n.
 * https://oeis.org/A101909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1024, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lower_bound_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A101909
 *  Number of primes between 2*n and 4*n (exclusive of both ends).
 *  Formula: a(n) = π(4n-1) - π(2n)
 *
 *  The implementation builds a sieve of Eratosthenes up to the
 *  required maximum (4*n − 1) on the first call and enlarges it if
 *  a larger n is requested.  The list of primes is kept in a
 *  static array and a binary search is used to count how many lie in
 *  the interval (2n,4n).  All arithmetic that must remain small is
 *  done in the native integer types; the result is cast to seq_t
 *  before returning.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/lower_bound_uint64.h"

/*  Memoised sieve data  */
static uint64_t *prime_list  = NULL;   /* array of primes */
static size_t    prime_cnt   = 0;      /* number of stored primes  */
static size_t    prime_cap   = 0;      /* allocated capacity for prime_list */
static uint64_t  sieve_limit = 0;      /* largest number that has been sieved (inclusive) */

/*  Sieve primes up to 'limit' (inclusive) and rebuild prime_list.  */
static void sieve_up_to(uint64_t limit)
{
    /* Allocate a bit array for primality test. */
    size_t sz = (size_t)(limit + 1);
    unsigned char *is_prime = (unsigned char *)malloc(sz);
    if (!is_prime)
        return;                       /* out of memory – caller will fail */

    /* Initialise as prime.  0 and 1 are not prime. */
    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Simple sieve using i*i <= limit to avoid sqrt. */
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[(size_t)i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[(size_t)j] = 0;
        }
    }

    /* Count primes to determine required capacity. */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            ++cnt;

    /* Resize prime list if necessary. */
    if (cnt > prime_cap) {
        uint64_t *new_arr = (uint64_t *)realloc(prime_list, cnt * sizeof(uint64_t));
        if (!new_arr) {
            free(is_prime);
            return;                   /* out of memory – caller will fail */
        }
        prime_list = new_arr;
        prime_cap  = cnt;
    }

    /* Fill prime list. */
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            prime_list[idx++] = i;

    prime_cnt   = cnt;
    sieve_limit = limit;

    free(is_prime);
}

/*  Ensure that the sieve covers at least 'limit' (inclusive).  */
static void ensure_sieve(uint64_t limit)
{
    if (sieve_limit < limit)
        sieve_up_to(limit);
}

/*  The required function.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined for n = 0 */

    uint64_t un   = (uint64_t)n;
    uint64_t low  = 2 * un;            /* exclusive lower bound (2n) */
    uint64_t high = 4 * un;            /* exclusive upper bound (4n) */

    /* We need primes < high, i.e. up to high-1. */
    ensure_sieve(high - 1);

    /* Find first prime > low and first prime >= high. */
    size_t idx_low  = lower_bound_uint64(prime_list, prime_cnt, low + 1);
    size_t idx_high = lower_bound_uint64(prime_list, prime_cnt, high);

    seq_t result = (seq_t)(idx_high - idx_low);
    return result;
}
