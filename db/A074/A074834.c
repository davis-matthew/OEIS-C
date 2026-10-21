/*
 * A074834  Primes whose base 4 reversal is also prime.
 * https://oeis.org/A074834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1318, u32 n <= 1319, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074834: primes whose base‑4 reversal is also prime.
   Computed by brute‑force search of successive integers, testing primality
   of the number itself and its base‑4 digit reversal. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Return the value obtained by reversing the digits of x when written in base‑4. */
static seq_t reverse_base4(seq_t x)
{
    seq_t rev = 0;
    while (x > (seq_t)0) {
        rev = rev * (seq_t)4 + (x % (seq_t)4);
        x   = x   / (seq_t)4;
    }
    return rev;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* stored terms */
    static size_t cache_sz  = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static seq_t cand = (seq_t)2;    /* next candidate to test */

    if (n <= (idx_t)cache_sz)
        return cache[n - 1];

    /* Extend the cache until we have at least n terms. */
    while (cache_sz < (size_t)n) {
        if (is_prime_big(cand) && is_prime_big(reverse_base4(cand))) {
            /* Ensure enough space. */
            if (cache_cap <= cache_sz) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache    = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = cand;
        }
        cand++;   /* next integer to test */
    }
    return cache[n - 1];
}
