/*
 * A068443  Triangular numbers which are the product of two primes.
 * https://oeis.org/A068443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 1259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A068443
   Formula: a(n) = n*(n+1)/2  for n such that T_k is the product of two distinct primes.
   Method: generate triangular numbers T_k = k*(k+1)/2; T_k is in the sequence iff
   (k even  → k/2 and k+1 are both prime) or
   (k odd   → k   and (k+1)/2 are both prime).
   The sequence is memoised so that calls with any n are answered in O(1) after the
   needed terms have been generated.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x) */

/* Static state for memoisation */
static idx_t next_k = 1;          /* next triangular index to test           */
static seq_t *cache   = NULL;     /* cached sequence values                  */
static size_t cache_len = 0;      /* number of terms stored                  */
static size_t cache_cap = 0;      /* allocated capacity                       */

/* enlarge the cache when needed */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    } else if (cache_len == cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
    }
}

/* return the nth term, 1‑based */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined: return 0 for safety */
        return (seq_t)0;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* decide whether T_{next_k} qualifies */
        bool ok = false;
        if (next_k % 2 == 0) {               /* even k : (k/2) and (k+1) must be prime */
            idx_t a1 = next_k / 2;
            idx_t b1 = next_k + 1;
            ok = is_prime(a1) && is_prime(b1);
        } else {                              /* odd k  : k and (k+1)/2 must be prime */
            idx_t a1 = next_k;
            idx_t b1 = (next_k + 1) / 2;
            ok = is_prime(a1) && is_prime(b1);
        }

        if (ok) {
            /* compute T_k = k*(k+1)/2 in idx_t to avoid overflow on small seq_t */
            idx_t k = next_k;
            seq_t T = (seq_t)(k * (k + 1) / 2);   /* conversion to seq_t occurs after full calculation */
            grow_cache();
            cache[cache_len++] = T;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
