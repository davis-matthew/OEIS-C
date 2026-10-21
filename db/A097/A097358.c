/*
 * A097358  Primes of the form 2*(p+q) + 1, p and q prime.
 * https://oeis.org/A097358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   isPrimeMiller
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of odd primes r such that (r-1)/2 is a sum of two primes (Goldbach). 
   The function enumerates odd numbers, tests primality, and checks a Goldbach representation
   of (r-1)/2. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeMiller.h"

/* Prime list storage for Goldbach checks */
static idx_t *prime_list = NULL;
static size_t prime_cnt = 0, prime_cap = 0;
static idx_t next_gen = 3;        /* next number to test for primality */

/* Ensure prime_list contains all primes <= limit */
static void primes_up_to(idx_t limit)
{
    if (next_gen > limit) return;
    if (!prime_list) {
        prime_cap = 16;
        prime_list = (idx_t*)malloc(prime_cap * sizeof(idx_t));
    }
    for (idx_t n = next_gen; n <= limit; ++n) {
        if (isPrimeMiller(n)) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                prime_list = (idx_t*)realloc(prime_list, prime_cap * sizeof(idx_t));
            }
            prime_list[prime_cnt++] = n;
        }
    }
    next_gen = limit + 1;
}

/* Check whether m can be written as the sum of two primes */
static bool goldbach(idx_t m)
{
    if (m <= 1) return false;
    if (m & 1) {            /* odd m: need 2 + (m-2) */
        if (m < 3) return false;
        return isPrimeMiller(m - 2);
    }
    idx_t half = m >> 1;
    primes_up_to(half);
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > half) break;
        idx_t q = m - p;
        if (isPrimeMiller(q)) return true;
    }
    return false;
}

/* Compute the nth term of A097358 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n = 0, safe fallback */

    idx_t count = 0;
    idx_t candidate = 3;             /* first odd prime to test */
    while (true) {
        if (!isPrimeMiller(candidate)) { candidate += 2; continue; }
        idx_t m = (candidate - 1) >> 1;   /* (candidate-1)/2 */
        if (goldbach(m)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        candidate += 2;
    }
}
