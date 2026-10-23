/*
 * A141075  Nonprimes of form (2*3*5*7*11-p), where p is prime.
 * https://oeis.org/A141075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 115, u32 n <= 115, u64 n <= 115,
 *            u128 n <= 115, big any n
 * Helpers:   build_primes_upto, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes nonprimes of the form 2310 - p, where p is a prime. */
/* The sequence ends with the 115th term a(115) = 2308. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_small.h"

static seq_t cache[115];
static bool   initialized = false;
static size_t term_count  = 0;

/* Helper: build the cache of sequence values. */
static void build_cache(void)
{
    uint64_t *primes = NULL;
    size_t    prime_cnt = 0;

    /* Generate all primes ≤ 2310 (inclusive). */
    build_primes_upto(2310u, &primes, &prime_cnt);

    /* Scan primes in descending order to obtain ascending values. */
    for (size_t i = prime_cnt; i != 0; )
    {
        --i;
        uint64_t p = primes[i];
        uint64_t v_int = 2310u - p;     /* 0 ≤ v_int ≤ 2308 */
        seq_t   v = (seq_t)v_int;

        /* Include 1 or any composite value. */
        if (v_int == 1 || !is_prime_small(v))
        {
            cache[term_count++] = v;
        }
    }

    free(primes);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        build_cache();

    if (n == 0 || (size_t)n > term_count)
        return (seq_t)0;    /* Undefined for out‑of‑range indices. */

    return cache[n - 1];
}
