/*
 * A155740  Odd numbers that are the concatenation of a prime p and the p-th
 *          nonprime nonnegative integer.
 * https://oeis.org/A155740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 1967, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155740:
   For each prime p, concatenate p with the p‑th non‑prime non‑negative integer.
   Keep only those results that are odd. a(n) is the nth such odd concatenation.  */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/concat_parts.h"

/* Cache for the sequence terms. */
static seq_t *cache = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;

/* State variables for generating terms incrementally. */
static idx_t next_prime_candidate = 2;   /* next integer to test for primality */
static idx_t cur_num = 0;                /* current integer being scanned for non‑prime */
static idx_t nonprime_counter = 0;       /* number of non‑primes seen so far */

/* Ensure the cache has space for at least `need` elements. */
static void grow_cache(size_t need)
{
    if (cache_cap < need) {
        size_t newcap = cache_cap ? cache_cap * 2 : 16;
        if (newcap < need) newcap = need;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }
}

/* Generate sequence terms until at least `n` terms have been produced. */
static void generate_up_to(idx_t n)
{
    while (cache_cnt < (size_t)n) {
        /* Find the next prime number. */
        while (!is_prime(next_prime_candidate)) ++next_prime_candidate;
        idx_t p = next_prime_candidate;          /* current prime */

        /* Find the p‑th non‑prime integer. */
        while (nonprime_counter < p) {
            if (!is_prime(cur_num)) ++nonprime_counter;
            ++cur_num;
        }
        idx_t Np = cur_num - 1;                  /* the p‑th non‑prime */

        /* Append the concatenation if the result is odd. */
        if (Np & 1ULL) {
            grow_cache(cache_cnt + 1);
            cache[cache_cnt++] = concat_parts(p, Np);
        }

        /* Prepare for the next iteration. */
        ++next_prime_candidate;
    }
}

/* Return the nth term of the sequence (a(1) is the first term).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset is 1, invalid index returns 0 */
    static bool initialized = false;
    if (!initialized) {
        /* Nothing to pre‑initialize; static variables already zeroed. */
        initialized = true;
    }
    generate_up_to(n);
    return cache[(size_t)n - 1];
}
