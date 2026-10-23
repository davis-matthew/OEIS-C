/*
 * A164622  Primes p such that p*floor(p/2) - 4 and p*floor(p/2) + 4 are prime
 *          numbers.
 * https://oeis.org/A164622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A164622.
   A prime p is accepted if both p·⌊p/2⌋−4 and p·⌊p/2⌋+4 are also prime.
*/
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"        /* bool is_prime(idx_t x); */
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t n); */

/* Cache the sequence values so that repeated calls to a(idx_t) are fast. */
static seq_t *cache = NULL;          /* array of stored terms */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t next_candidate = 3;     /* next odd number to test */
static idx_t found_count = 0;        /* number of terms found in cache */

static void grow_cache(idx_t needed)
{
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }
    while (cache_cap < needed) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* Undefined for n=0; return 0 as placeholder */

    /* Ensure the cache is large enough. */
    if ((size_t)n > cache_cap)
        grow_cache(n);

    /* Generate terms until we have at least n of them. */
    while (found_count < n) {
        if (is_prime(next_candidate)) {
            seq_t p = (seq_t)next_candidate;
            seq_t m = p >> 1;                 /* floor(p/2) */
            seq_t t1 = p * m - 4;
            seq_t t2 = p * m + 4;
            if (is_prime_big(t1) && is_prime_big(t2)) {
                cache[found_count] = p;
                found_count++;
            }
        }
        next_candidate += 2; /* check next odd number */
    }

    return cache[n - 1];
}
