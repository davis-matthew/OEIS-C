/*
 * A063909  Primes p such that 2*p - 5 is also prime.
 * https://oeis.org/A063909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p such that p is prime and 2*p-5 is prime (OEIS A063909)
 * Range:     u8 n <= 18, u16 n <= 1098, u32 n <= 9563, u64 n <= 9563,
 *            u128 n <= 9563, big any n
 * Helpers:   is_prime_uint64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/next_prime.h"      /* uint64_t next_prime(uint64_t start) */
#include "../utils/is_prime_uint64.h" /* bool is_prime_uint64(uint64_t n) */

static uint64_t *good_primes = NULL; /* cache of found primes          */
static idx_t      good_count  = 0;    /* number of primes stored       */
static idx_t      good_cap    = 0;    /* allocated capacity            */
static uint64_t   last_checked = 1;   /* last prime considered         */

static void ensure_good_capacity(void)
{
    if (good_count >= good_cap) {
        idx_t new_cap = good_cap ? good_cap << 1 : 16;
        uint64_t *tmp = (uint64_t *)realloc(good_primes,
                                            new_cap * sizeof(uint64_t));
        if (!tmp) abort();                 /* allocation failure */
        good_primes = tmp;
        good_cap    = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n==0, defensive */

    /* generate more terms until we have at least n of them */
    while (good_count < n) {
        last_checked = next_prime(last_checked + 1);
        uint64_t p = last_checked;
        uint64_t q = 2 * p - 5;
        if (q >= 2 && is_prime_uint64(q)) {
            ensure_good_capacity();
            good_primes[good_count++] = p;
        }
    }
    return (seq_t)good_primes[n - 1];
}
