/*
 * A154432  Numbers k such that 5k^2-k+1 is prime.
 * https://oeis.org/A154432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 32, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 5*k*k - k + 1 is prime.  Search is used. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;              /* cached sequence terms   */
static size_t cache_cap = 0;             /* capacity of the cache   */
static idx_t  cache_size = 0;            /* number of cached terms   */
static idx_t  last_k = 0;                /* last tested k value     */

/* Return the n-th term of the sequence (a(1) = 1). */
seq_t a(idx_t n)
{
    while (cache_size < n) {
        ++last_k;                                  /* next candidate k   */

        seq_t fk = (seq_t)last_k;                   /* cast k to seq_t    */
        seq_t f  = (seq_t)5 * fk * fk - fk + (seq_t)1; /* f(k) = 5k²-k+1 */

        if (is_prime_big(f)) {
            if (cache_size == cache_cap) {           /* grow cache      */
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();                   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = fk;                /* store the new k  */
        }
    }
    return cache[n - 1];
}
