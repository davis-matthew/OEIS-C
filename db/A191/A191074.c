/*
 * A191074  Primes p that have Kronecker symbol (p|55) = -1.
 * https://oeis.org/A191074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A191074: primes p with Kronecker symbol (p|55) = -1.
   For odd primes p ≠ 5,11: (p|55) = (p|5)*(p|11).
   (p|5) = 1 iff p ≡ 1 or 4 (mod 5), else –1.
   (p|11) = 1 iff p ≡ 1,3,4,5,9 (mod 11), else –1.
   The symbol equals –1 when the two values differ.
   Multiples of 5 or 11 are excluded because their symbol is 0. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"

/* cache for the sequence */
static seq_t *seq_cache = NULL;
static idx_t  seq_len   = 0;   /* number of values stored */
static idx_t  seq_cap   = 0;   /* capacity of seq_cache */

/* primes used for primality testing */
static uint64_t *prime_list = NULL;
static size_t   prime_len  = 0;   /* number of primes stored */
static size_t   prime_cap  = 0;   /* capacity of prime_list */
static uint64_t curr_candidate = 3;   /* next odd number to test */

/* add a prime to prime_list, expanding storage if necessary */
static void add_prime(uint64_t p)
{
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_len++] = p;
}

/* test whether x is prime using the primes in prime_list */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    uint64_t limit = (uint64_t)int_sqrt((idx_t)x);
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p > limit) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* ensure the cache holds at least target terms (target is 1‑based) */
static void expand_cache(idx_t target)
{
    if (seq_len >= target) return;

    /* initialise prime list if not yet done */
    if (prime_len == 0) {
        prime_cap   = 16;
        prime_list  = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) abort();
        add_prime(2);                 /* start with 2 */
        curr_candidate = 3;           /* next candidate to test */
    }

    /* extend cache until we have enough values */
    while (seq_len < target) {
        /* find next prime candidate */
        while (true) {
            if (is_prime_uint64(curr_candidate)) {
                /* record prime for future tests */
                add_prime(curr_candidate);

                /* exclusion rules */
                if (curr_candidate != 5 && curr_candidate != 11) {
                    int r5   = (int)(curr_candidate % 5);
                    int leg5 = (r5 == 1 || r5 == 4) ? 1 : -1;
                    int r11  = (int)(curr_candidate % 11);
                    int leg11 = (r11 == 1 || r11 == 3 || r11 == 4 ||
                                 r11 == 5 || r11 == 9) ? 1 : -1;
                    if (leg5 * leg11 == -1) {
                        /* store in cache, expanding if needed */
                        if (seq_len == seq_cap) {
                            size_t new_cap = seq_cap ? seq_cap << 1 : 16;
                            seq_t *tmp = (seq_t *)realloc(seq_cache,
                                                          new_cap * sizeof(seq_t));
                            if (!tmp) abort();
                            seq_cache = tmp;
                            seq_cap   = new_cap;
                        }
                        seq_cache[seq_len++] = (seq_t)curr_candidate;
                        curr_candidate += 2;   /* advance to next odd */
                        break;                /* go to outer loop */
                    }
                }
            }
            curr_candidate += 2;              /* next odd candidate */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; undefined for n==0 */
    expand_cache(n);
    return seq_cache[n - 1];
}
