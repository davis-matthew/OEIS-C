/*
 * A124669  Product of successive primes minus 2.
 * https://oeis.org/A124669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124669: Product of successive primes minus 2
   a(n) = p_n * p_{n+1} - 2 ,  offset 1 (a(1)=4) */

/* the build system provides the integer types seq_t and idx_t */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* store the primes that have been found */
static uint64_t *primes = NULL;      /* primes[0] = 2, primes[1] = 3, ... */
static size_t primes_cap = 0;        /* allocated capacity (number of slots) */
static size_t primes_cnt = 0;        /* number of primes actually stored */

/* resize the primes array to hold at least `need` elements */
static void grow_primes(size_t need)
{
    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;   /* double until big enough */

    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) {
        /* allocation failed – abort (memory exhausted) */
        exit(1);
    }
    primes = tmp;
    primes_cap = new_cap;
}

/* make sure there are at least `need` primes in the array */
static void ensure_primes(size_t need)
{
    if (primes_cnt >= need)
        return;

    /* initialise the array if empty */
    if (primes_cap == 0) {
        grow_primes(16);
        primes[0] = 2;
        primes_cnt = 1;
    }

    uint64_t candidate = primes[primes_cnt - 1] + 1;
    while (primes_cnt < need) {
        int is_prime = 1;
        /* trial division by all known primes up to sqrt(candidate) */
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            unsigned __int128 sq = (unsigned __int128)p * p;
            if (sq > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap)
                grow_primes(primes_cnt + 1);
            primes[primes_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* public sequence function – fulfills the required signature */
seq_t a(idx_t n)
{
    /* n is 1‑based; we need the first n+1 primes */
    ensure_primes((size_t)(n + 1));

    seq_t prod = (seq_t)primes[n - 1] * (seq_t)primes[n];
    return prod - (seq_t)2;
}
