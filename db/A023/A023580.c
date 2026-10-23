/*
 * A023580  Sum of distinct prime divisors of prime(n)+3.
 * https://oeis.org/A023580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum of distinct prime divisors of prime(n)+3
 * Range:     u8 n <= 54, u16 n <= 7804, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/factorize.h"

static uint32_t *prime_list = NULL;
static size_t prime_list_cap = 0;
static size_t prime_list_cnt = 0;
static uint32_t next_candidate = 2;

/* Build at least the first n primes (1‑based). */
static void ensure_nprimes(size_t n)
{
    if (prime_list_cnt >= n)
        return;

    if (!prime_list) {
        prime_list_cap = 64;
        prime_list = (uint32_t *)malloc(prime_list_cap * sizeof(uint32_t));
    }

    while (prime_list_cnt < n) {
        if (is_prime_small((seq_t)next_candidate)) {
            if (prime_list_cnt >= prime_list_cap) {
                prime_list_cap *= 2;
                prime_list = (uint32_t *)realloc(prime_list,
                                                prime_list_cap * sizeof(uint32_t));
            }
            prime_list[prime_list_cnt++] = next_candidate;
        }
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* undefined, but safe */

    ensure_nprimes((size_t)n);

    uint64_t val = (uint64_t)prime_list[(size_t)n - 1] + 3ULL;

    /* Factorise val; at most a few prime factors. */
    uint64_t primes[32];
    int      exps  [32];
    size_t   cnt   = factorize(val, primes, exps);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)primes[i];

    return sum;
}
