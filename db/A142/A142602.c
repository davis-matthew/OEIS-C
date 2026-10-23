/*
 * A142602  Primes congruent to 2 mod 55.
 * https://oeis.org/A142602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 2 (mod 55)
 * Range:     u8 n <= 2, u16 n <= 123, u32 n <= 135, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;      /* array of computed primes */
    static size_t cap   = 0;         /* allocated capacity */
    static size_t cnt   = 0;         /* number of terms stored */
    static idx_t  cand  = 2;         /* next candidate to test */

    if (n == 0) return 0;            /* offset is 1; n==0 is undefined */

    /* initialise cache on the first call */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failed – return a sentinel */
        cache[0] = (seq_t)2;         /* first term 2 */
        cnt   = 1;
        cand  = 57;                  /* start of the arithmetic progression 55*k+2, k≥1 */
    }

    /* generate terms until we reach the requested index */
    while (cnt < n) {
        /* grow the cache if necessary */
        if (cnt >= cap) {
            size_t newcap = cap << 1;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return 0;      /* allocation failed */
            cache = tmp;
            cap   = newcap;
        }

        /* test the current candidate */
        if (is_prime_big((seq_t)cand))
            cache[cnt++] = (seq_t)cand;

        /* next number congruent to 2 modulo 55 */
        cand += 55;
    }

    return cache[n-1];
}
