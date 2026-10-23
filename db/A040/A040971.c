/*
 * A040971  Primes p such that x^5 = 13 has no solution mod p.
 * https://oeis.org/A040971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040971: primes p for which x^5 = 13 has no solution modulo p.
   Computation is performed by iterating successive primes (via next_prime)
   and testing the condition 13^((p−1)/5) mod p ≠ 1 for primes with p ≡ 1 (mod 5). */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"

static seq_t *good_primes = NULL;   /* array of primes that satisfy the condition */
static size_t good_primes_cnt = 0;  /* current number of such primes */
static size_t good_primes_cap = 0;  /* allocated capacity of the array */
static uint64_t next_candidate = 2; /* next prime to examine */

/* Grow the dynamic array of good primes. */
static void grow_good_primes(void)
{
    size_t new_cap = good_primes_cap == 0 ? 16 : good_primes_cap * 2;
    seq_t *tmp = (seq_t *)realloc(good_primes, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);  /* memory allocation failed */
    good_primes = tmp;
    good_primes_cap = new_cap;
}

/* Ensure that at least `needed` good primes are stored. */
static void ensure_good_primes(idx_t needed)
{
    while (good_primes_cnt < needed) {
        uint64_t p = next_candidate;
        next_candidate = next_prime(p + 1);

        if (p % 5 != 1) continue;                      /* only primes ≡ 1 (mod 5) can fail */

        seq_t exp = (seq_t)(p - 1) / 5;                 /* exponent for the test */
        seq_t res = mod_pow((seq_t)13, (uint64_t)exp, (seq_t)p);

        if (res != (seq_t)1) {                          /* condition satisfied */
            if (good_primes_cnt == good_primes_cap)
                grow_good_primes();
            good_primes[good_primes_cnt++] = (seq_t)p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* defined only for n ≥ 1 */
    ensure_good_primes((idx_t)n);         /* generate up to the required index */
    return good_primes[(size_t)n - 1];    /* 0‑based indexing in the array */
}
