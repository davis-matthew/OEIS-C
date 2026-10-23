/*
 * A156655  Primes of the form 1000*k + 1.
 * https://oeis.org/A156655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form 1000*k + 1 for k ≥ 1
 * Range:     u8 none, u16 n <= 10, u32 n <= 14, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *prime_cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t next_k = 2;          /* last tested k; next test will be next_k + 1 */

static void ensure_cache(idx_t n)
{
    /* grow the array until we have at least n primes in the form 1000*k+1   */
    while (cache_len < (size_t)n) {
        next_k++;                                        /* new candidate k   */
        seq_t cand = ((seq_t)1000 * (seq_t)next_k) + (seq_t)1;
        if (is_prime_big(cand)) {                        /* test primality    */
            if (cache_len == cache_cap) {                /* extend storage    */
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(prime_cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) exit(1);                       /* out of memory    */
                prime_cache = tmp;
                cache_cap = new_cap;
            }
            prime_cache[cache_len++] = cand;              /* store prime      */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                        /* undefined for 0   */
    ensure_cache(n);                                    /* guarantee n primes*/
    return prime_cache[(size_t)n - 1];                  /* 1‑based indexing */
}
