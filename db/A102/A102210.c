/*
 * A102210  Number of primes that are bitwise covered by n.
 * https://oeis.org/A102210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{p prime ≤ n | (p & n) = p}
 * Range:     u8 n <= 2046, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   add_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of primes p ≤ n whose set bits are all contained in n.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* is_prime(uint64_t) */
#include "../utils/add_prime.h"  /* add_prime(uint64_t **, size_t *, size_t *, uint64_t) */

static uint64_t *prime_arr   = NULL;   /* dynamic array of primes */
static size_t      prime_cap = 0;      /* array capacity */
static size_t      prime_cnt = 0;      /* number of stored primes */
static idx_t       last_checked = 0;   /* largest integer checked for primality */

/* Ensure that all primes ≤ n are generated and stored in prime_arr. */
static void
ensure_primes(idx_t n)
{
    /* Generate primes from last_checked+1 up to n by trial division. */
    for (idx_t i = last_checked + 1; i <= n; ++i) {
        if (is_prime(i))
            add_prime(&prime_arr, &prime_cap, &prime_cnt, (uint64_t)i);
    }
    last_checked = n;
}

seq_t
a(idx_t n)
{
    if (n < 2) return 0;          /* no primes ≤ 1 */

    /* Generate necessary primes once and keep them for future calls. */
    ensure_primes(n);

    seq_t count = 0;
    uint64_t mask = (uint64_t)n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        if ((prime_arr[i] & mask) == prime_arr[i])
            ++count;
    }
    return count;
}
