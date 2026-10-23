/*
 * A116617  Values of n such that prime(2n+1) mod 12 = 11.
 * https://oeis.org/A116617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n such that prime(2*n+1) ≡ 11 (mod 12)
 * Range:     u8 n <= 9, u16 n <= 953, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static idx_t candidate    = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for offset 0 */

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        /* find the next n that satisfies the condition */
        while (1) {
            seq_t prime = (seq_t)get_prime(2 * candidate + 1);
            if ((prime % 12) == 11) break;   /* condition met */
            candidate++;                     /* test next integer */
        }

        /* ensure capacity and store the found candidate */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size++] = candidate;
        candidate++;                         /* start next search after this n */
    }

    return cache[n - 1];   /* sequences are 1‑based */
}
